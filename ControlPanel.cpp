#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

using namespace sf;

// функция загрузки текстуры
bool loadTexture(Texture& texture, const std::string& fileName)
{
	if (!texture.loadFromFile("res/" + fileName))
	{
		std::cout << "Can't find the image" << fileName << std::endl;
		return false;
	}
	return true;
}

// создание спрайта
Sprite createSprite(Texture& texture, float x, float y)
{
	Sprite sprite;
	sprite.setTexture(texture);
	sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
	sprite.setPosition(x + texture.getSize().x / 2.0f, y + texture.getSize().y / 2.0f);
	return sprite;
}

// обработка нажатия на кнопку
void handleButtonPress(Sprite& button, bool& state, Texture& onTexture, Texture& offTexture)
{
	state = !state;
	if (state)
	{
		button.setTexture(onTexture);
	}
	else
	{
		button.setTexture(offTexture);
	}
}

int main()
{
	RenderWindow window(VideoMode(900, 700), "PULT", Style::Default);
	Font font;
	if (!font.loadFromFile("consolas.ttf")) {
		std::cout << "Can't find the font file" << std::endl;
	}

	// состояния кнопок
	bool switchButtonState = false;
	bool switchButtonState2 = true;
	bool circleSwitchButtonState = false;
	bool circleSwitchButtonState2 = true;
	bool isStereoState = false;
	bool isPausedState = false;
	bool isRecordingState = false;
	Sprite* lastSprite = nullptr;

	Texture switchButtonOn;
	loadTexture(switchButtonOn, "legend_of_pult_switch_on.png");

	Texture switchButtonOff;
	loadTexture(switchButtonOff, "legend_of_pult_switch_off.png");

	Texture circleSwitchButtonOn;
	loadTexture(circleSwitchButtonOn, "legend_of_pult_circle_switch_on.png");

	Texture circleSwitchButtonOff;
	loadTexture(circleSwitchButtonOff, "legend_of_pult_circle_switch_off.png");

	Texture potentiometerOn;
	loadTexture(potentiometerOn, "legend_of_pult_potentiometer_on.png");

	Texture potentiometerOff;
	loadTexture(potentiometerOff, "legend_of_pult_potentiometer_off.png");

	Texture canalSwitchBack;
	loadTexture(canalSwitchBack, "legend_of_pult_canal_switch_back.png");

	Texture canalSwitchButton;
	loadTexture(canalSwitchButton, "legend_of_pult_canal_switch_button.png");

	Texture controlPanelBack;
	loadTexture(controlPanelBack, "legend_of_pult_panel_back.png");

	Texture controlPanelButton;
	loadTexture(controlPanelButton, "legend_of_pult_simple_button.png");

	// Текстуры для ползунка
	Texture lineSwitchTexture;
	loadTexture(lineSwitchTexture, "legend_line_switch.png");

	Texture scrollerTexture;
	loadTexture(scrollerTexture, "legend_of_scroller.png");

	Texture controlPanelSignatures;
	loadTexture(controlPanelSignatures, "control_panel_signatures.png");

	Sprite switchButtonImage = createSprite(switchButtonOff, 380.0f, 280.0f);
	Sprite switchButtonImage2 = createSprite(switchButtonOn, 510.0f, 280.0f);
	Sprite circleSwitchButtonImage = createSprite(circleSwitchButtonOff, 380.0f, 120.0f);
	Sprite circleSwitchButtonImage2 = createSprite(circleSwitchButtonOn, 510.0f, 120.0f);
	Sprite potentiometerOnImage = createSprite(potentiometerOn, 90.0f, 150.0f);
	Sprite potentiometerOffImage = createSprite(potentiometerOff, 230.0f, 150.0f);
	Sprite canalSwitchBackImage = createSprite(canalSwitchBack, 95.0f, 340.0f);
	Sprite canalSwitchMonoImage = createSprite(canalSwitchButton, 100.0f, 345.0f);
	Sprite canalSwitchStereoImage = createSprite(canalSwitchButton, 190.0f, 345.0f);
	Sprite controlPanelBackImage = createSprite(controlPanelBack, 95.0f, 500.0f);
	Sprite pauseButtonImage = createSprite(controlPanelButton, 100.0f, 505.0f);
	Sprite playButtonImage = createSprite(controlPanelButton, 190.0f, 505.0f);
	Sprite prevButtonImage = createSprite(controlPanelButton, 280.0f, 505.0f);
	Sprite nextButtonImage = createSprite(controlPanelButton, 370.0f, 505.0f);
	Sprite stopButtonImage = createSprite(controlPanelButton, 460.0f, 505.0f);
	Sprite startButtonImage = createSprite(controlPanelButton, 550.0f, 505.0f);
	Sprite controlPanelSignaturesImage = createSprite(controlPanelSignatures, 95, 600);
	startButtonImage.setColor(Color(255, 0, 0, 150));

	CircleShape canalSwitchIndicator(10.0f);
	canalSwitchIndicator.setFillColor(Color(7, 247, 215));
	canalSwitchIndicator.setOrigin(Vector2f(10, 10));
	canalSwitchIndicator.setPosition(100.0f + canalSwitchButton.getSize().x / 2.0f, 345.0f + canalSwitchButton.getSize().y / 2.0f);

	// Переменные для управления вращением circleSwitchButton
	float potentiometerOnAngle = 0.0f;
	float potentiometerOffAngle = 0.0f;
	bool isRotating = false;

	// Создание спрайтов для ползунка
	Sprite lineSwitchImage = createSprite(lineSwitchTexture, 800.0f, 200.0f); // Подняли полоску на уровень круглых кнопок
	Sprite scrollerImage = createSprite(scrollerTexture, 800.0f, 200.0f); // Подняли ползунок на уровень круглых кнопок

	// Переменные для управления ползунком
	bool isDraggingScroller = false;
	float scrollerMinY = lineSwitchImage.getPosition().y - lineSwitchTexture.getSize().y / 2.0f + scrollerTexture.getSize().y / 2.0f;
	float scrollerMaxY = lineSwitchImage.getPosition().y + lineSwitchTexture.getSize().y / 2.0f - scrollerTexture.getSize().y / 2.0f;

	// Создание прямоугольника для экрана
	RectangleShape screenBackground(Vector2f(880.0f, 100.0f));
	screenBackground.setFillColor(Color(0, 0, 0));
	screenBackground.setPosition(10.0f, 10.0f);

	// Создание текстового объекта для экрана
	Text screenText;
	screenText.setFont(font);
	screenText.setCharacterSize(24);
	screenText.setFillColor(Color::White);
	screenText.setPosition(20.0f, 20.0f);

	Text monoText;
	monoText.setFont(font);
	monoText.setCharacterSize(20);
	monoText.setFillColor(Color::Black);
	monoText.setPosition(120.0f, 440.0f);
	monoText.setString("mono");

	Text stereoText;
	stereoText.setFont(font);
	stereoText.setCharacterSize(20);
	stereoText.setFillColor(Color::Black);
	stereoText.setPosition(200.0f, 440.0f);
	stereoText.setString("stereo");


	while (window.isOpen())
	{
		Event Event;
		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::MouseButtonPressed:
			{
				Vector2i mousePos = Mouse::getPosition(window);
				Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				if (switchButtonImage.getGlobalBounds().contains(mousePosF))
				{
					handleButtonPress(switchButtonImage, switchButtonState, switchButtonOn, switchButtonOff);
					screenText.setString("Switch Button 1: " + switchButtonState ? "On" : "Off");
				}
				else if (circleSwitchButtonImage.getGlobalBounds().contains(mousePosF))
				{
					handleButtonPress(circleSwitchButtonImage, circleSwitchButtonState, circleSwitchButtonOn, circleSwitchButtonOff);
					screenText.setString("Circle Switch Button 1: " + circleSwitchButtonState ? "On" : "Off");
				}
				if (switchButtonImage2.getGlobalBounds().contains(mousePosF))
				{
					handleButtonPress(switchButtonImage2, switchButtonState2, switchButtonOn, switchButtonOff);
					screenText.setString("Switch Button 2: " + switchButtonState2 ? "On" : "Off");
				}
				else if (circleSwitchButtonImage2.getGlobalBounds().contains(mousePosF))
				{
					handleButtonPress(circleSwitchButtonImage2, circleSwitchButtonState2, circleSwitchButtonOn, circleSwitchButtonOff);
					screenText.setString("Circle Switch Button 2: " + circleSwitchButtonState2 ? "On" : "Off");
				}
				else if (canalSwitchMonoImage.getGlobalBounds().contains(mousePosF))
				{
					lastSprite = &canalSwitchMonoImage;
					canalSwitchMonoImage.scale(Vector2f(0.9f, 0.9f));
					if (isStereoState) {
						canalSwitchIndicator.setPosition(100.0f + canalSwitchButton.getSize().x / 2.0f, 345.0f + canalSwitchButton.getSize().y / 2.0f);
						isStereoState = !isStereoState;
					}
					canalSwitchIndicator.scale(Vector2f(0.9f, 0.9f));
					screenText.setString("Canal Switch: Mono");
				}
				else if (canalSwitchStereoImage.getGlobalBounds().contains(mousePosF))
				{
					lastSprite = &canalSwitchStereoImage;
					canalSwitchStereoImage.scale(Vector2f(0.9f, 0.9f));
					if (!isStereoState) {
						canalSwitchIndicator.setPosition(190.0f + canalSwitchButton.getSize().x / 2.0f, 345.0f + canalSwitchButton.getSize().y / 2.0f);
						isStereoState = !isStereoState;
					}
					canalSwitchIndicator.scale(Vector2f(0.9f, 0.9f));
					screenText.setString("Canal Switch: Stereo");
				}
				else if (pauseButtonImage.getGlobalBounds().contains(mousePosF))
				{
					if (!isPausedState) {
						pauseButtonImage.scale(Vector2f(0.9f, 0.9f));
						isPausedState = !isPausedState;
					}
					screenText.setString("Pause Button: Paused");
				}
				else if (playButtonImage.getGlobalBounds().contains(mousePosF))
				{
					lastSprite = &playButtonImage;
					playButtonImage.scale(Vector2f(0.9f, 0.9f));
					if (isPausedState) {
						pauseButtonImage.scale(Vector2f(10 / 9.0f, 10 / 9.0f));
						isPausedState = !isPausedState;
					}
					screenText.setString("Play Button: Playing");
				}
				else if (prevButtonImage.getGlobalBounds().contains(mousePosF))
				{
					lastSprite = &prevButtonImage;
					prevButtonImage.scale(Vector2f(0.9f, 0.9f));
					screenText.setString("Previous Button: Pressed");
				}
				else if (nextButtonImage.getGlobalBounds().contains(mousePosF))
				{
					lastSprite = &nextButtonImage;
					nextButtonImage.scale(Vector2f(0.9f, 0.9f));
					screenText.setString("Next Button: Pressed");
				}
				else if (stopButtonImage.getGlobalBounds().contains(mousePosF))
				{
					lastSprite = &stopButtonImage;
					stopButtonImage.scale(Vector2f(0.9f, 0.9f));
					if (isRecordingState) {
						startButtonImage.scale(Vector2f(10 / 9.0f, 10 / 9.0f));
						isRecordingState = !isRecordingState;
					}
					screenText.setString("Stop Button: Stopped");
				}
				else if (startButtonImage.getGlobalBounds().contains(mousePosF))
				{
					if (!isRecordingState) {
						startButtonImage.scale(Vector2f(0.9f, 0.9f));
						isRecordingState = !isRecordingState;
					}
					screenText.setString("Start Button: Recording");
				}
				else if (potentiometerOnImage.getGlobalBounds().contains(mousePosF))
				{
					isRotating = true;
				}
				else if (scrollerImage.getGlobalBounds().contains(mousePosF))
				{
					isDraggingScroller = true;
				}
			}
			break;
			case Event::MouseButtonReleased:
			{
				Vector2i mousePos = Mouse::getPosition(window);
				Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

				if (lastSprite && lastSprite->getGlobalBounds().contains(mousePosF))
				{
					lastSprite->setScale(Vector2f(1.0f, 1.0f));
					if (!isStereoState && lastSprite == &canalSwitchMonoImage)
					{
						canalSwitchIndicator.setScale(Vector2f(1.0f, 1.0f));
					}
					else if (isStereoState && lastSprite == &canalSwitchStereoImage)
					{
						canalSwitchIndicator.setScale(Vector2f(1.0f, 1.0f));
					}
					lastSprite = nullptr;
				}
				if (isRotating)
				{
					isRotating = false;
				}
				if (isDraggingScroller)
				{
					isDraggingScroller = false;
				}
				break;
			}
			case Event::MouseMoved:
			{
				Vector2i mousePos = Mouse::getPosition(window);
				Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

				if (lastSprite && lastSprite != &startButtonImage && lastSprite != &stopButtonImage && !lastSprite->getGlobalBounds().contains(mousePosF))
				{
					lastSprite->setScale(Vector2f(1.0f, 1.0f));
					if (!isStereoState && lastSprite == &canalSwitchMonoImage)
					{
						canalSwitchIndicator.setScale(Vector2f(1.0f, 1.0f));
					}
					else if (isStereoState && lastSprite == &canalSwitchStereoImage)
					{
						canalSwitchIndicator.setScale(Vector2f(1.0f, 1.0f));
					}
					lastSprite = nullptr;
				}
				else if (lastSprite == &stopButtonImage && !stopButtonImage.getGlobalBounds().contains(mousePosF))
				{
					stopButtonImage.setScale(Vector2f(1.0f, 1.0f));
					lastSprite = nullptr;
				}
				if (isDraggingScroller)
				{
					float newY = mousePosF.y;
					if (newY < scrollerMinY)
					{
						newY = scrollerMinY;
					}
					else if (newY > scrollerMaxY)
					{
						newY = scrollerMaxY;
					}
					scrollerImage.setPosition(814.0f, newY);
				}
				break;
			}
			case Event::MouseWheelScrolled:
			{
				if (potentiometerOnImage.getGlobalBounds().contains(static_cast<Vector2f>(Mouse::getPosition(window))))
				{
					potentiometerOnAngle -= Event.mouseWheelScroll.delta * 10.0f;

					if (potentiometerOnAngle < 0)
					{
						potentiometerOnAngle = 0;
					}
					else if (potentiometerOnAngle > 360)
					{
						potentiometerOnAngle = 360;
					}

					potentiometerOnImage.setRotation(potentiometerOnAngle);
				}
				else if (potentiometerOffImage.getGlobalBounds().contains(static_cast<Vector2f>(Mouse::getPosition(window))))
				{
					potentiometerOffAngle -= Event.mouseWheelScroll.delta * 10.0f;

					if (potentiometerOffAngle < 0)
					{
						potentiometerOffAngle = 0;
					}
					else if (potentiometerOffAngle > 360)
					{
						potentiometerOffAngle = 360;
					}

					potentiometerOffImage.setRotation(potentiometerOffAngle);
				}
				break;
			}
			}
		}

		window.clear(Color(175, 175, 175));
		window.draw(screenBackground); // Рисуем фон экрана
		window.draw(screenText); // Рисуем текст экрана
		window.draw(monoText);
		window.draw(stereoText);
		window.draw(switchButtonImage);
		window.draw(circleSwitchButtonImage);
		window.draw(switchButtonImage2);
		window.draw(circleSwitchButtonImage2);
		window.draw(potentiometerOnImage);
		window.draw(potentiometerOffImage);
		window.draw(canalSwitchBackImage);
		window.draw(canalSwitchMonoImage);
		window.draw(canalSwitchStereoImage);
		window.draw(canalSwitchIndicator);
		window.draw(controlPanelBackImage);
		window.draw(pauseButtonImage);
		window.draw(playButtonImage);
		window.draw(prevButtonImage);
		window.draw(nextButtonImage);
		window.draw(stopButtonImage);
		window.draw(startButtonImage);
		window.draw(lineSwitchImage);
		window.draw(scrollerImage);
		window.draw(controlPanelSignaturesImage);
		window.display();
	}
	return 0;
}
