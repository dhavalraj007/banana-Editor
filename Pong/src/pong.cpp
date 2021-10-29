#include"banana/engine.h"
#include"banana/main.h"
#include"banana/log.h"
#include"banana/input/keyboard.h"
#include"banana/graphics/framebuffer.h"
#include"external/imgui/imgui.h"
#include"factory.h"

using namespace banana;

class Pong :public App
{
public:
	
	std::shared_ptr<GameObject> m_PadLeft,m_PadRight;
	std::shared_ptr<Ball> m_Ball;
	std::shared_ptr<GameObject> m_BG;
	int m_LeftPaddleUp, m_LeftPaddleDown, m_RightPaddleUp, m_RightPaddleDown;
	float m_PadSpeed;
	float m_BallSpeed;
	float m_BallMult=0.1f;
	bool BallCollisonRight;
	int leftScore = 0;
	int rightScore = 0;
	int m_PointsTowin = 5;
	bool inGamePlay = false;
	std::string winnerName;

	core::WindowProperties getWindowProperties()	override
	{
		core::WindowProperties props;
		props.w = 800;
		props.h = 600;
		props.imguiProps.isDockingEnable = true;
		return props;
	}

	void initialize() override
	{
		m_BG = factory::createBG();
		m_PadLeft = factory::createPaddle({ -1.0f,0.f });
		m_PadRight= factory::createPaddle({  1.0f,0.f  });
		m_Ball = factory::createBall({ 0.f,0.f  });

		m_LeftPaddleUp = BANANA_INPUT_KEY_W;
		m_LeftPaddleDown = BANANA_INPUT_KEY_S;
		m_RightPaddleUp = BANANA_INPUT_KEY_UP;
		m_RightPaddleDown = BANANA_INPUT_KEY_DOWN;
		
		m_PadLeft->Update();
		m_PadRight->Update();
		m_Ball->Update();
		reset(false);
		
	}
	void update() override
	{

		if (input::Keyboard::keyDown(BANANA_INPUT_KEY_ESCAPE))
		{
			Engine::Instance().quit();
			return;
		}
		m_BG->Update();
		if (inGamePlay)
		{
			m_PadLeft->Update();
			m_PadRight->Update();
			m_Ball->Update();
			if (input::Keyboard::key(m_LeftPaddleUp) && m_PadLeft->getPos().y < 1 - m_PadLeft->getSize().y / 2)
			{
				m_PadLeft->move({ 0.f,m_PadSpeed });
			}
			if (input::Keyboard::key(m_LeftPaddleDown) && m_PadLeft->getPos().y > -1 + m_PadLeft->getSize().y / 2)
			{
				m_PadLeft->move({ 0.f,-m_PadSpeed });
			}
			if (input::Keyboard::key(m_RightPaddleUp) && m_PadRight->getPos().y < 1 - m_PadRight->getSize().y / 2)
			{
				m_PadRight->move({ 0.f,m_PadSpeed });
			}
			if (input::Keyboard::key(m_RightPaddleDown) && m_PadRight->getPos().y > -1 + m_PadRight->getSize().y / 2)
			{
				m_PadRight->move({ 0.f,-m_PadSpeed });
			}

			m_Ball->move(m_Ball->getVel());
			if (m_Ball->getPos().y > 1 - m_Ball->getSize().y / 2 || m_Ball->getPos().y < -1 + m_Ball->getSize().y / 2)
			{
				m_Ball->flipVelY();
			}

			if (IsColliding(m_Ball->getPos(), m_Ball->getSize(), m_PadLeft->getPos(), m_PadLeft->getSize()) && !BallCollisonRight)
			{
				m_Ball->flipVelX();
				BallCollisonRight = !BallCollisonRight;

				handlePaddleCollision(m_LeftPaddleUp, m_LeftPaddleDown);
			}

			if (IsColliding(m_Ball->getPos(), m_Ball->getSize(), m_PadRight->getPos(), m_PadRight->getSize()) && BallCollisonRight)
			{
				m_Ball->flipVelX();
				BallCollisonRight = !BallCollisonRight;

				handlePaddleCollision(m_RightPaddleUp, m_RightPaddleDown);
			}

			if (m_Ball->getPos().x - m_Ball->getSize().x / 2 < -1.f)
			{
				rightScore++;
				reset(false);
				if (rightScore >= m_PointsTowin)
				{
					winnerName = "Right wins!";
					inGamePlay = false;
				}
			}
			if (m_Ball->getPos().x + m_Ball->getSize().x / 2 > 1.f)
			{
				leftScore++;
				reset(true);

				if (leftScore >= m_PointsTowin)
				{
					winnerName = "Left wins!";
					inGamePlay = false;
				}
			}
		}
	}
	void shutdown() override
	{ 

	}
	void render() override
	{
		m_BG->Render();
		m_Ball->Render();
		m_PadLeft->Render();
		m_PadRight->Render();
	}

	void imguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::Begin("Score"))
		{
			ImGui::Text("left score : %d", leftScore);
			ImGui::Text("right score : %d", rightScore);
			ImGui::Text("Ball velocity: X:%.5f Y:%.5f", m_Ball->getVel().x,m_Ball->getVel().y);
		}
		ImGui::End();
		if (ImGui::Begin("Game View"))
		{
			if (ImGui::IsWindowHovered())
			{
				ImGui::CaptureMouseFromApp(false);
				ImGui::CaptureKeyboardFromApp(false);
			}
			ImGui::SetCursorPosX(20);
			ImVec2 size{ 640,480};
			ImVec2 uv0{ 0,1 };
			ImVec2 uv1{ 1,0 };
			auto& window = Engine::Instance().getWindow();
			ImGui::Image((ImTextureID)(intptr_t)window.getFramebuffer()->getTextureId(), size, uv0, uv1);
		}
		ImGui::End();

		if(ImGui::Begin("Menu"))
		{
			if (!inGamePlay)
			{
				if (ImGui::Button("Start Game"))
				{
					startGame();
				}
				ImGui::Text("%s ", winnerName.c_str());
			}
		}
		ImGui::End();
	}
private:
	bool IsColliding(const glm::vec2& posA, const glm::vec2& sizeA, const glm::vec2& posB, const glm::vec2& sizeB)
	{
		float Aleft		= posA.x - sizeA.x / 2;
		float ARight	= posA.x + sizeA.x / 2;
		float ATop		= posA.y + sizeA.y / 2;
		float ABot		= posA.y - sizeA.y / 2;

		float Bleft		= posB.x - sizeB.x / 2;
		float BRight	= posB.x + sizeB.x / 2;
		float BTop		= posB.y + sizeB.y / 2;
		float BBot		= posB.y - sizeB.y / 2;

		return ((Aleft < BRight&& ARight > Bleft) &&
				(ATop  > BBot  && BTop > ABot));
	}

	void startGame()
	{
		leftScore = 0;
		rightScore = 0;
		inGamePlay = true;
		winnerName = "";	
	}

	void handlePaddleCollision(int upkey, int downkey)
	{
		if (m_Ball->getVel().y > 0 && input::Keyboard::key(upkey) ||
			(m_Ball->getVel().y < 0 && input::Keyboard::key(downkey)))
		{
			m_Ball->setVelocity(m_Ball->getVel() * (1 + m_BallMult));
		}
		else if (m_Ball->getVel().y < 0 && input::Keyboard::key(upkey) ||
			(m_Ball->getVel().y > 0 && input::Keyboard::key(downkey)))
		{
			m_Ball->setVelocity(m_Ball->getVel() * (1 - m_BallMult));
		}
	}
	void reset(bool leftScored)
	{
		
		m_PadSpeed = 0.001f;
		m_BallSpeed = 0.0005f;
		BallCollisonRight = leftScored;
		if (leftScored)
		{
			m_Ball->setVelocity({ m_BallSpeed,m_BallSpeed });
		}
		else
		{
			m_Ball->setVelocity({ -m_BallSpeed,m_BallSpeed });
		}
		m_Ball->setPos({ 0.f,0.f });
		m_PadLeft->setPos({ -1.0f,0.f });
		m_PadRight->setPos({ 1.0f,0.f });
	}
};

App* createApp()
{
	return new Pong();
}
