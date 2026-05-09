#include "gui.h"
#include <iostream>
#include <algorithm>
#include <cmath>

GUI::GUI(Simulation& s)
    : sim(s), 
      window(sf::VideoMode(sf::Vector2u(1400, 1000)), "HYPER-FLOW ULTIMATE") 
{
    // تحديد فريمات ثابتة للقضاء على الجليتش
    window.setFramerateLimit(60); 

    // محاولة تحميل الخط من أكتر من مسار لضمان ظهوره
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (!font.openFromFile("arial.ttf")) {
            std::cerr << "Font Error: Please put arial.ttf in the project folder!" << std::endl;
        }
    }

    visualHistory.assign(100, 0);
    bgColor = sf::Color(8, 10, 14);
    normalCustomerColor = sf::Color(0, 255, 255);
    vipCustomerColor = sf::Color(255, 215, 0); 
    freeServerColor = sf::Color(50, 255, 50);
    busyServerColor = sf::Color(255, 20, 100);
}

void GUI::drawGlowCircle(float x, float y, float radius, sf::Color color, int intensity) {
    for (int i = intensity; i > 0; --i) {
        sf::CircleShape glow(radius + i * 2.5f);
        glow.setOrigin(sf::Vector2f(glow.getRadius(), glow.getRadius()));
        glow.setPosition(sf::Vector2f(x, y));
        glow.setFillColor(sf::Color(color.r, color.g, color.b, 120 / i));
        window.draw(glow);
    }
    sf::CircleShape core(radius);
    core.setOrigin(sf::Vector2f(radius, radius));
    core.setPosition(sf::Vector2f(x, y));
    core.setOutlineThickness(3.5f);
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

    window.clear(bgColor);
    drawDashboard();
    drawQueue();
    drawServers(); 
    drawGraph();
    window.display(); 
}

void GUI::drawServers() {
    const auto& servers = sim.getServers();
    int displayLimit = std::min((int)servers.size(), 3); 
    float lerpSpeed = 0.1f;

    float serverX = 1220.f; 
    float startY = 220.f;
    float spacingY = 280.f;

    for (int i = 0; i < displayLimit; ++i) {
        float y = startY + (i * spacingY); 
        bool isFree = servers[i].isFree();
        sf::Color sCol = isFree ? freeServerColor : busyServerColor;
        
        sf::RectangleShape card(sf::Vector2f(280.f, 250.f)); 
        card.setOrigin(sf::Vector2f(140.f, 125.f));
        card.setPosition(sf::Vector2f(serverX, y));
        card.setFillColor(sf::Color(12, 14, 18));
        card.setOutlineThickness(2.5f);
        card.setOutlineColor(sCol);
        window.draw(card);

        if (!isFree && servers[i].getCurrentCustomer()) {
            Customer* c = servers[i].getCurrentCustomer();
            int id = c->getId();
            
            if (customerPositions.find(id) == customerPositions.end()) {
                customerPositions[id] = {serverX - 150.f, y}; 
            }
            
            customerPositions[id].x += (serverX - customerPositions[id].x) * lerpSpeed;
            customerPositions[id].y += (y - 35.f - customerPositions[id].y) * lerpSpeed;

            drawGlowCircle(customerPositions[id].x, customerPositions[id].y, 38.f, 
                           c->getIsVIP() ? vipCustomerColor : normalCustomerColor, 5);

            sf::Text idTxt(font, std::to_string(id), 22);
            idTxt.setOrigin(idTxt.getLocalBounds().getCenter());
            idTxt.setPosition(sf::Vector2f(customerPositions[id].x, customerPositions[id].y));
            idTxt.setFillColor(sf::Color::White);
            window.draw(idTxt);
            
            sf::Text tLeft(font, "PROCESS: " + std::to_string(servers[i].getRemainingTime()) + "s", 18);
            tLeft.setOrigin(sf::Vector2f(tLeft.getLocalBounds().getCenter().x, 0.f));
            tLeft.setPosition(sf::Vector2f(serverX, y + 65.f));
            tLeft.setFillColor(sCol);
            window.draw(tLeft);
        } else {
            sf::Text rTxt(font, "READY", 26);
            rTxt.setOrigin(rTxt.getLocalBounds().getCenter());
            rTxt.setPosition(sf::Vector2f(serverX, y));
            rTxt.setFillColor(sCol);
            window.draw(rTxt);
        }
    }
}

void GUI::drawQueue() {
    const auto& q = sim.getQueue();
    int limit = std::min((int)q.getQueueSize(), 10);
    float lerpSpeed = 0.18f; // سرعة استجابة أعلى للأنيميشن
    float t = animClock.getElapsedTime().asSeconds();

    for (int i = 0; i < limit; ++i) {
        Customer* c = q.getCustomerAt(i);
        if (!c) continue;
        int id = c->getId();

        float targetX = 950.f - (i * 115.f); // مسافة تباعد كافية
        float targetY = 520.f + std::sin(t * 3.5f + i) * 8.f;

        if (customerPositions.find(id) == customerPositions.end()) {
            customerPositions[id] = {targetX - 120.f, targetY}; 
        }

        customerPositions[id].x += (targetX - customerPositions[id].x) * lerpSpeed;
        customerPositions[id].y += (targetY - customerPositions[id].y) * lerpSpeed;

        drawGlowCircle(customerPositions[id].x, customerPositions[id].y, 36.f, 
                       c->getIsVIP() ? vipCustomerColor : normalCustomerColor, 4);

        sf::Text idTxt(font, std::to_string(id), 18);
        idTxt.setOrigin(idTxt.getLocalBounds().getCenter());
        idTxt.setPosition(sf::Vector2f(customerPositions[id].x, customerPositions[id].y));
        idTxt.setFillColor(sf::Color::White);
        window.draw(idTxt);
    }
}

void GUI::drawDashboard() {
    sf::RectangleShape bar(sf::Vector2f(1400.f, 110.f));
    bar.setFillColor(sf::Color(18, 22, 28));
    window.draw(bar);
    
    sf::Text title(font, "HYPER-FLOW ULTIMATE", 26);
    title.setPosition(sf::Vector2f(50.f, 35.f));
    title.setFillColor(sf::Color::White);
    window.draw(title);

    sf::Text timer(font, "SYSTEM UPTIME: " + std::to_string(sim.getCurrentTime()) + "s", 32);
    timer.setPosition(sf::Vector2f(550.f, 32.f));
    timer.setFillColor(sf::Color(0, 255, 255));
    window.draw(timer);
}

void GUI::drawGraph() {
    float gX = 60.f, gY = 700.f, gW = 950.f, gH = 260.f;
    sf::RectangleShape bg(sf::Vector2f(gW, gH));
    bg.setPosition(sf::Vector2f(gX, gY));
    bg.setFillColor(sf::Color(12, 14, 18, 240));
    bg.setOutlineThickness(1.5f);
    bg.setOutlineColor(sf::Color(0, 255, 255, 40));
    window.draw(bg);

    if (visualHistory.empty()) return;
    int maxVal = *std::max_element(visualHistory.begin(), visualHistory.end());
    float scale = (maxVal < 6) ? 6.0f : (float)maxVal;

    sf::VertexArray glowArea(sf::PrimitiveType::TriangleStrip, visualHistory.size() * 2);
    for (size_t i = 0; i < visualHistory.size(); ++i) {
        float x = gX + (i * (gW / (visualHistory.size() - 1)));
        float y = gY + gH - ((float)visualHistory[i] / scale * (gH - 50.f));
        glowArea[i*2].position = sf::Vector2f(x, gY + gH);
        glowArea[i*2].color = sf::Color(0, 255, 255, 10);
        glowArea[i*2+1].position = sf::Vector2f(x, y);
        glowArea[i*2+1].color = sf::Color(0, 255, 255, 90); 
    }
    window.draw(glowArea);
}

bool GUI::isOpen() const { return window.isOpen(); }
void GUI::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();
    }
}