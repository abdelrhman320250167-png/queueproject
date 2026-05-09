#include "gui.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>

GUI::GUI(Simulation& s)
    : sim(s), 
      window(sf::VideoMode({1200, 900}), "HYPER-FLOW NEON PRO V16.0"), 
      tickRate(1.0f) // ثانية كاملة عشان العداد يمشي طبيعي
{
    srand((unsigned int)time(0));
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf") && !font.openFromFile("arial.ttf")) {
        std::cerr << "Font Error!" << std::endl;
    }
    visualHistory.assign(100, 0);
    
    // ألوان النيون اللي بتنور في الضلمة
    bgColor = sf::Color({10, 12, 16});
    normalCustomerColor = sf::Color({0, 255, 255}); // Cyan
    vipCustomerColor = sf::Color({255, 0, 255});   // Magenta
    freeServerColor = sf::Color({50, 255, 50});    // Green
    busyServerColor = sf::Color({255, 20, 100});   // Red
}

void GUI::render(Simulation& s) {
    // --- تحديث السميوليشن (القلب النابض) ---
    if (clock.getElapsedTime().asSeconds() >= tickRate) {
        // احتمالية دخول الناس (رجعتها 45% عشان الأكشن)
        if (rand() % 100 < 45) { 
            int randomID = rand() % 900 + 100;
            Customer* c = new Customer(randomID, sim.getCurrentTime(), rand()%7+4, (rand()%10==0));
            Queue& q = const_cast<Queue&>(sim.getQueue());
            q.enqueueCustomer(c);
        }
        const_cast<Simulation&>(sim).runTick(); 
        visualHistory.push_back(sim.getQueue().getQueueSize());
        if (visualHistory.size() > 100) visualHistory.erase(visualHistory.begin());
        clock.restart(); 
    }

    window.clear(bgColor);
    float t = animClock.getElapsedTime().asSeconds();
    
    drawDashboard();
    drawQueue();
    drawServers(); 
    drawGraph();

    // تأثير الليزر النيون
    float scanPos = std::fmod(t * 400.f, 1200.f);
    sf::RectangleShape scan({3.f, 900.f});
    scan.setPosition({scanPos, 0.f});
    scan.setFillColor({0, 255, 255, 20});
    window.draw(scan);
    
    window.display(); 
}

void GUI::drawDashboard() {
    sf::RectangleShape bar({1200.f, 100.f});
    bar.setFillColor({20, 24, 30});
    window.draw(bar);
    
    sf::Text title(font, "NEON FLOW MONITOR", 24);
    title.setPosition({40.f, 32.f});
    title.setFillColor(sf::Color::White);
    window.draw(title);

    // العداد - رجع كبير وواضح في النص
    sf::Text timer(font, "SYSTEM UPTIME: " + std::to_string(sim.getCurrentTime()) + "s", 32);
    timer.setPosition({480.f, 28.f});
    timer.setFillColor({0, 255, 255});
    window.draw(timer);

    sf::Text qStats(font, "QUEUE: " + std::to_string(sim.getQueue().getQueueSize()), 18);
    qStats.setPosition({980.f, 38.f});
    qStats.setFillColor(vipCustomerColor);
    window.draw(qStats);
}

void GUI::drawServers() {
    const auto& servers = sim.getServers();
    int displayLimit = std::min((int)servers.size(), 3); 

    for (int i = 0; i < displayLimit; ++i) {
        float x = 930.f, y = 140.f + (i * 245.f); 
        bool isFree = servers[i].isFree();
        sf::Color sCol = isFree ? freeServerColor : busyServerColor;
        
        sf::RectangleShape card({240.f, 210.f}); 
        card.setPosition({x, y});
        card.setFillColor({15, 15, 20});
        card.setOutlineThickness(3.f);
        card.setOutlineColor(sCol);
        window.draw(card);

        if (!isFree && servers[i].getCurrentCustomer()) {
            sf::Text idTxt(font, "USER #" + std::to_string(servers[i].getCurrentCustomer()->getId()), 26);
            idTxt.setPosition({x + 25.f, y + 60.f});
            idTxt.setFillColor(sf::Color::White);
            window.draw(idTxt);
            
            sf::Text tLeft(font, "PROCESS: " + std::to_string(servers[i].getRemainingTime()) + "s", 16);
            tLeft.setPosition({x + 25.f, y + 135.f});
            tLeft.setFillColor(sCol);
            window.draw(tLeft);
        } else {
            sf::Text rTxt(font, "READY", 22);
            rTxt.setPosition({x + 75.f, y + 85.f});
            rTxt.setFillColor(sCol);
            window.draw(rTxt);
        }
    }
}

void GUI::drawQueue() {
    const auto& q = sim.getQueue();
    int limit = std::min((int)q.getQueueSize(), 8);
    float t = animClock.getElapsedTime().asSeconds();

    for (int i = 0; i < limit; ++i) {
        Customer* c = q.getCustomerAt(i);
        if (!c) continue;
        
        float x = 810.f - (i * 95.f);
        float y = 440.f + std::sin(t * 5.f + i) * 10.f; 
        
        sf::Color col = c->getIsVIP() ? vipCustomerColor : normalCustomerColor;
        sf::CircleShape circle(34.f);
        circle.setPosition({x, y});
        circle.setOutlineThickness(4.f); 
        circle.setOutlineColor(col);
        circle.setFillColor({10, 10, 15});
        window.draw(circle);

        sf::Text id(font, std::to_string(c->getId()), 18);
        id.setPosition({x + 16.f, y + 20.f});
        id.setFillColor(col);
        window.draw(id);
    }
}

void GUI::drawGraph() {
    // الجراف - رجعناه كبير جداً ومرفوع
    float gX = 40.f, gY = 580.f, gW = 860.f, gH = 290.f;
    
    sf::RectangleShape bg({gW, gH});
    bg.setPosition({gX, gY});
    bg.setFillColor({15, 18, 22, 230});
    bg.setOutlineThickness(1.f);
    bg.setOutlineColor({0, 255, 255, 60});
    window.draw(bg);

    if (visualHistory.empty()) return;

    int maxVal = *std::max_element(visualHistory.begin(), visualHistory.end());
    // سكيل حساس (5) عشان الجراف يرفع فوراً
    float scale = (maxVal < 5) ? 5.0f : (float)maxVal;

    sf::VertexArray glowArea(sf::PrimitiveType::TriangleStrip, visualHistory.size() * 2);
    for (size_t i = 0; i < visualHistory.size(); ++i) {
        float x = gX + (i * (gW / (visualHistory.size() - 1)));
        float y = gY + gH - ((float)visualHistory[i] / scale * (gH - 50.f));
        
        glowArea[i*2].position = {x, gY + gH};
        glowArea[i*2].color = {0, 255, 255, 10};
        glowArea[i*2+1].position = {x, y};
        glowArea[i*2+1].color = {0, 255, 255, 100}; 
    }
    window.draw(glowArea);

    sf::VertexArray mainLine(sf::PrimitiveType::LineStrip, visualHistory.size());
    for (size_t i = 0; i < visualHistory.size(); ++i) {
        float x = gX + (i * (gW / (visualHistory.size() - 1)));
        float y = gY + gH - ((float)visualHistory[i] / scale * (gH - 50.f));
        mainLine[i].position = {x, y};
        mainLine[i].color = {0, 255, 255};
    }
    window.draw(mainLine);
}

void GUI::run() { window.setFramerateLimit(60); }
bool GUI::isOpen() const { return window.isOpen(); }
void GUI::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();
    }
}