#include "gui.h"
#include "Customer.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>

GUI::GUI(Simulation& s)
    : sim(s), 
      // في نسخة 2.6 مش بنحتاج Vector2u هنا
      window(sf::VideoMode(1400, 1000), "E-JUST QUEUE MONITOR") 
{
    window.setFramerateLimit(60); 

    // في نسخة 2.6 الدالة اسمها loadFromFile مش openFromFile
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf") && !font.loadFromFile("arial.ttf")) {
        std::cerr << "Font Error: Please ensure arial.ttf is in the project folder!" << std::endl;
    }

    visualHistory.assign(100, 0);
    bgColor = sf::Color(8, 10, 14);
    normalCustomerColor = sf::Color(0, 255, 255);
    vipCustomerColor = sf::Color(255, 215, 0); 
    freeServerColor = sf::Color(50, 255, 50);
    busyServerColor = sf::Color(255, 20, 100);
}

void GUI::drawGlowCircle(float x, float y, float radius, sf::Color color) {
    sf::CircleShape glow(radius + 4.f);
    glow.setOrigin(radius + 4.f, radius + 4.f);
    glow.setPosition(x, y);
    glow.setFillColor(sf::Color(color.r, color.g, color.b, 60));
    window.draw(glow);

    sf::CircleShape core(radius);
    core.setOrigin(radius, radius);
    core.setPosition(x, y);
    core.setOutlineThickness(3.0f);
    core.setOutlineColor(color);
    core.setFillColor(sf::Color(15, 15, 20));
    window.draw(core);
}

void GUI::render(Simulation& s) {
    if (clock.getElapsedTime().asSeconds() >= 0.5f) {
        visualHistory.push_back(sim.getQueue().getQueueSize());
        if (visualHistory.size() > 100) visualHistory.erase(visualHistory.begin());
        clock.restart();
    }

    std::set<int> activeIDs;
    for (int i = 0; i < sim.getQueue().getQueueSize(); ++i) {
        if (sim.getQueue().getCustomerAt(i)) activeIDs.insert(sim.getQueue().getCustomerAt(i)->getId());
    }
    for (const auto& server : sim.getServers()) {
        if (!server.isFree() && server.getCurrentCustomer()) activeIDs.insert(server.getCurrentCustomer()->getId());
    }

    for (auto it = customerPositions.begin(); it != customerPositions.end(); ) {
        if (activeIDs.find(it->first) == activeIDs.end()) it = customerPositions.erase(it);
        else ++it;
    }

    window.clear(bgColor);
    drawDashboard();
    drawQueue();
    drawServers(); 
    drawGraph();
    window.display(); 
}

void GUI::drawServers() {
    const auto& servers = sim.getServers();
    float lerpSpeed = 0.08f; 
    float serverX = 1220.f; 
    float startY = 220.f;
    float spacingY = 280.f;

    for (int i = 0; i < std::min((int)servers.size(), 3); ++i) {
        float y = startY + (i * spacingY); 
        bool isFree = servers[i].isFree();
        sf::Color sCol = isFree ? freeServerColor : busyServerColor;
        
        sf::RectangleShape card(sf::Vector2f(280.f, 240.f)); 
        card.setOrigin(140.f, 120.f);
        card.setPosition(serverX, y);
        card.setFillColor(sf::Color(12, 14, 18));
        card.setOutlineThickness(2.0f);
        card.setOutlineColor(sCol);
        window.draw(card);

        if (!isFree && servers[i].getCurrentCustomer()) {
            int id = servers[i].getCurrentCustomer()->getId();
            if (customerPositions.find(id) == customerPositions.end()) customerPositions[id] = {950.f, 520.f};

            customerPositions[id].x += (serverX - customerPositions[id].x) * lerpSpeed;
            customerPositions[id].y += (y - 30.f - customerPositions[id].y) * lerpSpeed;

            drawGlowCircle(customerPositions[id].x, customerPositions[id].y, 38.f, normalCustomerColor);

            // الطريقة القديمة بتاعة SFML 2.6 لضبط النص
            sf::Text idTxt;
            idTxt.setFont(font);
            idTxt.setString(std::to_string(id));
            idTxt.setCharacterSize(22);
            sf::FloatRect textBounds = idTxt.getLocalBounds();
            idTxt.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
            idTxt.setPosition(customerPositions[id].x, customerPositions[id].y);
            idTxt.setFillColor(sf::Color::White);
            window.draw(idTxt);
        }
    }
}

void GUI::drawQueue() {
    const auto& q = sim.getQueue();
    int limit = std::min((int)q.getQueueSize(), 8);
    float lerpSpeed = 0.1f;

    for (int i = 0; i < limit; ++i) {
        Customer* c = q.getCustomerAt(i);
        if (!c) continue;
        int id = c->getId();

        float targetX = 900.f - (i * 155.f); 
        float targetY = 520.f; 

        if (customerPositions.find(id) == customerPositions.end()) customerPositions[id] = {targetX - 50.f, targetY};

        customerPositions[id].x += (targetX - customerPositions[id].x) * lerpSpeed;
        customerPositions[id].y += (targetY - customerPositions[id].y) * lerpSpeed;

        drawGlowCircle(customerPositions[id].x, customerPositions[id].y, 36.f, normalCustomerColor);

        sf::Text idTxt;
        idTxt.setFont(font);
        idTxt.setString(std::to_string(id));
        idTxt.setCharacterSize(18);
        sf::FloatRect textBounds = idTxt.getLocalBounds();
        idTxt.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        idTxt.setPosition(customerPositions[id].x, customerPositions[id].y);
        idTxt.setFillColor(sf::Color::White);
        window.draw(idTxt);
    }
}

void GUI::drawDashboard() {
    sf::RectangleShape bar(sf::Vector2f(1400.f, 100.f));
    bar.setFillColor(sf::Color(18, 22, 28));
    window.draw(bar);
    
    sf::Text timer;
    timer.setFont(font);
    timer.setString("SYSTEM UPTIME: " + std::to_string(sim.getCurrentTime()) + "s");
    timer.setCharacterSize(30);
    timer.setPosition(550.f, 28.f);
    timer.setFillColor(sf::Color(0, 255, 255));
    window.draw(timer);
}

void GUI::drawGraph() {
    float gX = 60.f, gY = 720.f, gW = 950.f, gH = 240.f;
    if (visualHistory.empty()) return;
    
    sf::VertexArray glowArea(sf::TriangleStrip, visualHistory.size() * 2); // PrimitiveType اتغيرت برضه
    for (size_t i = 0; i < visualHistory.size(); ++i) {
        float x = gX + (i * (gW / (visualHistory.size() - 1)));
        float y = gY + gH - ((float)visualHistory[i] / 10.f * (gH - 40.f));
        glowArea[i*2].position = sf::Vector2f(x, gY + gH);
        glowArea[i*2].color = sf::Color(0, 255, 255, 10);
        glowArea[i*2+1].position = sf::Vector2f(x, y);
        glowArea[i*2+1].color = sf::Color(0, 255, 255, 80); 
    }
    window.draw(glowArea);
}

bool GUI::isOpen() const { return window.isOpen(); }

void GUI::handleEvents() {
    sf::Event event;
    // الـ Event Loop بتاع 2.6
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}