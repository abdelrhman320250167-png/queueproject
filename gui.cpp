#include "gui.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

SimulationGUI::SimulationGUI(Simulation& s) 
    : sim(s), 
      window(sf::VideoMode({1200, 900}), "QUANTUM QUEUE SYSTEMS - V4.0"), 
      tickRate(1.0f) 
{
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf") && !font.openFromFile("arial.ttf")) {
        std::cerr << "Font Error!" << std::endl;
    }

    // الألوان النيون الفخمة
    bgColor = sf::Color(10, 12, 16);             // أسود عميق جداً
    normalCustomerColor = sf::Color(0, 255, 255); // Cyan Neon
    vipCustomerColor = sf::Color(255, 0, 255);    // Pink/Magenta Neon
    freeServerColor = sf::Color(0, 255, 128);     // Green Neon
    busyServerColor = sf::Color(255, 45, 85);     // Red Neon
}

void SimulationGUI::run() {
    window.setFramerateLimit(60); // عشان الحركة تبقى ناعمة
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void SimulationGUI::handleEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) window.close();
    }
}

void SimulationGUI::update() {
    if (clock.getElapsedTime().asSeconds() >= tickRate) {
        if (sim.getIsRunning()) sim.runTick(); 
        clock.restart(); 
    }
}

void SimulationGUI::render() {
    window.clear(bgColor); 
    
    // رسم شبكة خفيفة في الخلفية (Grid) عشان تدي منظر تكنولوجي
    for(int i=0; i<1200; i+=50) {
        sf::RectangleShape line({1.f, 900.f});
        line.setPosition({(float)i, 0.f});
        line.setFillColor(sf::Color(255,255,255, 10)); // خطوط شفافة جداً
        window.draw(line);
    }

    drawDashboard();
    drawQueue();
    drawServers();
    drawGraph();
    
    window.display(); 
}

void SimulationGUI::drawDashboard() {
    // Header Bar بخط نيون تحتيه
    sf::RectangleShape bar({1200.f, 80.f});
    bar.setFillColor(sf::Color(20, 24, 30));
    window.draw(bar);

    sf::RectangleShape neonLine({1200.f, 2.f});
    neonLine.setPosition({0.f, 80.f});
    neonLine.setFillColor(sf::Color(0, 255, 255, 150));
    window.draw(neonLine);

    sf::Text t(font);
    t.setFillColor(sf::Color::White);
    t.setStyle(sf::Text::Bold);
    t.setString("QUANTUM CONTROL INTERFACE");
    t.setCharacterSize(22);
    t.setPosition({30.f, 25.f});
    window.draw(t);

    int time = std::max(0, sim.getCurrentTime());
    std::string stats = "SYSTEM_UPTIME: " + std::to_string(time) + "s  |  LOAD: STABLE";
    t.setString(stats);
    t.setCharacterSize(16);
    t.setStyle(sf::Text::Regular);
    t.setFillColor(sf::Color(0, 255, 255));
    t.setPosition({850.f, 30.f});
    window.draw(t);
}

void SimulationGUI::drawServers() {
    const auto& servers = sim.getServers();
    for (int i = 0; i < (int)servers.size(); ++i) {
        float x = 930.f, y = 120.f + (i * 200.f);
        bool isFree = servers[i].isFree();
        sf::Color statusColor = isFree ? freeServerColor : busyServerColor;

        // رسم "هالة" Glow ورا السيرفر
        sf::RectangleShape glow({230.f, 160.f});
        glow.setPosition({x-5.f, y-5.f});
        glow.setFillColor(sf::Color(statusColor.r, statusColor.g, statusColor.b, 30));
        window.draw(glow);

        // المربع الأساسي (LCD Look)
        sf::RectangleShape card({220.f, 150.f});
        card.setPosition({x, y});
        card.setFillColor(sf::Color(15, 15, 20));
        card.setOutlineThickness(2.f);
        card.setOutlineColor(statusColor);
        window.draw(card);

        // Header صغير جوه الكارت
        sf::RectangleShape head({220.f, 30.f});
        head.setPosition({x, y});
        head.setFillColor(sf::Color(statusColor.r, statusColor.g, statusColor.b, 80));
        window.draw(head);

        sf::Text t(font);
        t.setFillColor(sf::Color::White);
        t.setString("STATION_UNIT_0" + std::to_string(i + 1));
        t.setCharacterSize(14);
        t.setPosition({x + 15.f, y + 5.f});
        window.draw(t);

        t.setString(isFree ? "STATUS: STANDBY" : "STATUS: ACTIVE");
        t.setFillColor(statusColor);
        t.setPosition({x + 15.f, y + 45.f});
        window.draw(t);

        if (!isFree && servers[i].getCurrentCustomer()) {
            t.setString("USER_ID: #" + std::to_string(servers[i].getCurrentCustomer()->getId()));
            t.setCharacterSize(26);
            t.setFillColor(sf::Color::White);
            t.setPosition({x + 15.f, y + 80.f});
            window.draw(t);
        }
    }
}

void SimulationGUI::drawQueue() {
    const auto& q = sim.getQueue();
    // خط الطابور بقى "ليزر"
    sf::RectangleShape laser({800.f, 2.f});
    laser.setPosition({50.f, 500.f});
    laser.setFillColor(sf::Color(0, 255, 255, 100));
    window.draw(laser);

    for (int i = 0; i < q.getQueueSize(); ++i) {
        Customer* c = q.getCustomerAt(i);
        if (!c) continue;
        float x = 750.f - (i * 90.f), y = 475.f;
        sf::Color cColor = c->getIsVIP() ? vipCustomerColor : normalCustomerColor;

        // Glow للعميل
        sf::CircleShape glow(30.f);
        glow.setPosition({x-5.f, y-5.f});
        glow.setFillColor(sf::Color(cColor.r, cColor.g, cColor.b, 40));
        window.draw(glow);

        sf::CircleShape person(25.f);
        person.setPosition({x, y});
        person.setFillColor(sf::Color(20, 20, 25));
        person.setOutlineThickness(3.f);
        person.setOutlineColor(cColor);
        window.draw(person);

        sf::Text id(font);
        id.setString(std::to_string(c->getId()));
        id.setCharacterSize(16);
        id.setFillColor(cColor);
        id.setPosition({x + 16.f, y + 14.f});
        window.draw(id);
    }
}

void SimulationGUI::drawGraph() {
    const auto& history = sim.getQueueHistory();
    float gX = 50.f, gY = 650.f, gW = 800.f, gH = 200.f;
    
    // خلفية الجراف بقت "Grid"
    sf::RectangleShape bg({gW, gH});
    bg.setPosition({gX, gY});
    bg.setFillColor(sf::Color(255, 255, 255, 5));
    window.draw(bg);

    if (history.size() < 2) return;
    sf::VertexArray line(sf::PrimitiveType::LineStrip, history.size());
    int maxQ = *std::max_element(history.begin(), history.end());
    if (maxQ == 0) maxQ = 1;
    for (size_t i = 0; i < history.size(); ++i) {
        float x = gX + (i * (gW / (history.size() - 1)));
        float y = gY + gH - ((float)history[i] / maxQ * (gH - 20.f));
        line[i].position = {x, y};
        line[i].color = sf::Color(0, 255, 255);
    }
    window.draw(line);
}