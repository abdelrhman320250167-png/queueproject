#include "Gui.h"

Gui::Gui(Simulation* sim)
    : simulationRef(sim),
    window(sf::VideoMode({1200, 900}), "Modern Queue Simulation", sf::State::Windowed),
      // In SFML 3, sf::Text must be initialized with the font in the constructor
    pauseText(font, "PAUSE", 22) 
{
    window.setFramerateLimit(60);
    
    // SFML 3 uses openFromFile instead of loadFromFile
    if (!font.openFromFile("arial.ttf")) {
        // Font missing handling
    }

    pauseButton.setSize(sf::Vector2f(100.f, 45.f));
    pauseButton.setPosition(sf::Vector2f(1060.f, 25.f));
    pauseButton.setFillColor(sf::Color(40, 45, 60, 200));
    pauseButton.setOutlineThickness(2.f);
    pauseButton.setOutlineColor(sf::Color(0, 212, 255));
    
    pauseText.setPosition(sf::Vector2f(1072.f, 32.f));
    pauseText.setFillColor(sf::Color::White);
}

void Gui::initializeWindow() {}

void Gui::connectToSimulation(Simulation* sim) { simulationRef = sim; }

bool Gui::isOpen() const { return window.isOpen(); }

void Gui::closeWindow() { window.close(); }

void Gui::handleEvents() {
    // SFML 3 Event loop uses std::optional
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        // Handling mouse clicks in SFML 3
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f pos(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));
                if (pauseButton.getGlobalBounds().contains(pos) && simulationRef) {
                    simulationRef->togglePause();
                    pauseText.setString(simulationRef->getIsPaused() ? "RESUME" : "PAUSE");
                    pauseButton.setOutlineColor(simulationRef->getIsPaused() ? sf::Color(255, 170, 0) : sf::Color(0, 212, 255));
                }
            }
        }
    }
}

void Gui::refreshDisplay() {
    window.clear(sf::Color(18, 20, 28)); 

    if (simulationRef) {
        drawDashboard();
        drawServers();
        drawQueue();
        drawGraph();
    }

    window.draw(pauseButton);
    window.draw(pauseText);

    // Modern smooth cinematic scanline effect
    float t = animClock.getElapsedTime().asSeconds();
    float scanPos = std::fmod(t * 600.f, 2400.f) - 600.f; 
    sf::RectangleShape scan(sf::Vector2f(150.f, 900.f));
    scan.setPosition(sf::Vector2f(scanPos, 0.f));
    
    scan.setFillColor(sf::Color(0, 212, 255, 8)); 
    window.draw(scan);
    
    window.display();
}

void Gui::drawDashboard() {
    sf::RectangleShape bar(sf::Vector2f(1200.f, 90.f));
    bar.setFillColor(sf::Color(30, 34, 45, 230));
    window.draw(bar);

    sf::Text title(font, "QUEUE DASHBOARD", 26);
    title.setPosition(sf::Vector2f(40.f, 30.f));
    title.setFillColor(sf::Color(255, 255, 255, 240));
    window.draw(title);

    sf::Text timer(font, "TIME: " + std::to_string(simulationRef->getCurrentTime()) + "s", 28);
    timer.setPosition(sf::Vector2f(500.f, 28.f));
    timer.setFillColor(sf::Color(0, 212, 255)); 
    window.draw(timer);

    sf::Text qStats(font, "WAITING: " + std::to_string(simulationRef->getQueue().getQueueSize()), 20);
    qStats.setPosition(sf::Vector2f(880.f, 35.f));
    qStats.setFillColor(sf::Color(255, 51, 102)); 
    window.draw(qStats);
}

void Gui::drawServers() {
    const auto& servers = simulationRef->getServers();
    int displayLimit = static_cast<int>(std::min<std::size_t>(servers.size(), 3));
    float t = animClock.getElapsedTime().asSeconds();
    
    float pulse = (std::sin(t * 6.f) + 1.f) / 2.f;

    for (int i = 0; i < displayLimit; ++i) {
        float x = 910.f;
        float y = 130.f + i * 250.f;
        bool free = servers[i].isFree();
        
        sf::Color readyColor = sf::Color(46, 204, 113); 
        sf::Color busyColor = sf::Color(255, 51, 102);  
        // SFML 3 uses standard types like std::uint8_t
        sf::Color outlineColor = free ? readyColor : sf::Color(255, 51, 102, 100 + static_cast<std::uint8_t>(155 * pulse));

        sf::RectangleShape card(sf::Vector2f(250.f, 220.f));
        card.setPosition(sf::Vector2f(x, y));
        card.setFillColor(sf::Color(30, 34, 45));
        card.setOutlineThickness(free ? 2.f : 4.f); 
        card.setOutlineColor(outlineColor);
        window.draw(card);

        if (!free && servers[i].getCurrentCustomer()) {
            sf::Text idTxt(font, "User ID: #" + std::to_string(servers[i].getCurrentCustomer()->getId()), 24);
            idTxt.setPosition(sf::Vector2f(x + 30.f, y + 60.f));
            idTxt.setFillColor(sf::Color::White);
            window.draw(idTxt);

            sf::Text tLeft(font, "Processing: " + std::to_string(servers[i].getRemainingTime()) + "s", 18);
            tLeft.setPosition(sf::Vector2f(x + 30.f, y + 140.f));
            tLeft.setFillColor(busyColor);
            window.draw(tLeft);
        } else {
            sf::Text rTxt(font, "STANDBY", 26);
            rTxt.setPosition(sf::Vector2f(x + 65.f, y + 90.f));
            rTxt.setFillColor(readyColor);
            window.draw(rTxt);
        }
    }
}

void Gui::drawQueue() {
    const auto& q = simulationRef->getQueue();
    int limit = static_cast<int>(std::min(8, q.getQueueSize()));
    float t = animClock.getElapsedTime().asSeconds();

    for (int i = 0; i < limit; ++i) {
        Customer* c = q.getCustomerAtIndex(i);
        if (!c) continue;
        
        float x = 760.f - i * 100.f;
        float hoverY = std::sin(t * 4.f + i) * 8.f; 
        float y = 440.f + hoverY;
        
        sf::Color vipColor = sf::Color(255, 170, 0); 
        sf::Color normColor = sf::Color(0, 212, 255); 
        sf::Color col = c->getIsVIP() ? vipColor : normColor;

        sf::CircleShape glow(40.f);
        glow.setPosition(sf::Vector2f(x - 6.f, y - 6.f));
        glow.setFillColor(sf::Color(col.r, col.g, col.b, 30));
        window.draw(glow);

        sf::CircleShape circle(34.f);
        circle.setPosition(sf::Vector2f(x, y));
        circle.setOutlineThickness(3.f);
        circle.setOutlineColor(col);
        circle.setFillColor(sf::Color(30, 34, 45));
        window.draw(circle);

        sf::Text id(font, std::to_string(c->getId()), 20);
        id.setPosition(sf::Vector2f(x + 12.f, y + 16.f)); // Static offset for stability
        id.setFillColor(col);
        window.draw(id);
    }
}

void Gui::drawGraph() {
    const auto& history = simulationRef->getQueueHistory();
    float gX = 40.f, gY = 580.f, gW = 820.f, gH = 290.f;

    sf::RectangleShape bg(sf::Vector2f(gW, gH));
    bg.setPosition(sf::Vector2f(gX, gY));
    bg.setFillColor(sf::Color(25, 28, 38, 200));
    bg.setOutlineThickness(1.f);
    bg.setOutlineColor(sf::Color(0, 212, 255, 80));
    window.draw(bg);

    if (history.empty()) return;
    
    int maxVal = 0;
    for (int val : history) {
        if (val > maxVal) maxVal = val;
    }
    float scale = (maxVal < 5) ? 5.0f : static_cast<float>(maxVal);

    // SFML 3 uses sf::PrimitiveType::TriangleStrip
    sf::VertexArray glowArea(sf::PrimitiveType::TriangleStrip, history.size() * 2);
    for (std::size_t i = 0; i < history.size(); ++i) {
        float x = gX + (i * (gW / (static_cast<float>(history.size()) - 1)));
        float y = gY + gH - (history[i] / scale * (gH - 40.f));
        
        glowArea[i * 2].position = sf::Vector2f(x, gY + gH);
        glowArea[i * 2].color = sf::Color(0, 212, 255, 0); 
        
        glowArea[i * 2 + 1].position = sf::Vector2f(x, y);
        glowArea[i * 2 + 1].color = sf::Color(0, 212, 255, 90); 
    }
    window.draw(glowArea);

    sf::VertexArray line(sf::PrimitiveType::LineStrip, history.size());
    float lastX = 0, lastY = 0;
    for (std::size_t i = 0; i < history.size(); ++i) {
        float x = gX + (i * (gW / (static_cast<float>(history.size()) - 1)));
        float y = gY + gH - (history[i] / scale * (gH - 40.f));
        line[i].position = sf::Vector2f(x, y);
        line[i].color = sf::Color(0, 212, 255);
        lastX = x;
        lastY = y;
    }
    window.draw(line);

    if (!history.empty()) {
        float t = animClock.getElapsedTime().asSeconds();
        float pulse = (std::sin(t * 8.f) + 1.f) / 2.f;
        sf::CircleShape liveDot(6.f);
        liveDot.setOrigin(sf::Vector2f(6.f, 6.f));
        liveDot.setPosition(sf::Vector2f(lastX, lastY));
        liveDot.setFillColor(sf::Color(255, 255, 255));
        liveDot.setOutlineThickness(3.f + (2.f * pulse));
        liveDot.setOutlineColor(sf::Color(0, 212, 255, 150));
        window.draw(liveDot);
    }
}