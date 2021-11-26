#include <iostream>
#include <vector>
#include <string>
#include "banana/engine.h"
#include"banana/app.h"
#include"banana/main.h"
#include"banana/log.h"


#include<pqxx/pqxx>

#include"banana/core/assetlibrary.h"

#include"banana/graphics/vertex.h"
#include"banana/graphics/shader.h"
#include"banana/graphics/texture.h"
#include"banana/graphics/framebuffer.h"
#include"banana/graphics/camera.h"

#include"banana/input/mouse.h"
#include"banana/input/keyboard.h"
#include"external/imgui/imgui.h"

#define GLM_ENABLE_EXPERIMENTAL

using namespace banana;

class Editor :public banana::App
{
public:

	bool m_ImguiEnabled = true;
	std::string connectionString = "host=localhost port=5432 dbname=gamezone user=postgres password =dhaval";
	pqxx::connection connectionObject{ connectionString.c_str() };
	pqxx::result data; 
	pqxx::result tables; 
	pqxx::work worker{ connectionObject };
	int numOfRows;

	std::string rname;
	int rage;
	int dID;
	std::string rcnumber;
	char rbuffer1[120] = "";
	char rbuffer2[120] = "";

	int tgid;
	std::string tname;
	std::string tdate;
	char tbuffer1[120] = "";
	char tbuffer2[120] = "";
	int trewards;
	int tsid;

	std::string eq1 = "1. Find details of staff who have solved at least one complaintand has salary less than average.";
	std::string eq2 = "2. List all users who have won more than one tournament.";
	std::string eq3 = "3. Find the average price of all paid games.";
	core::WindowProperties getWindowProperties()
	{
		core::WindowProperties props;
		props.title = "pgtest";
		props.w = 854;
		props.h = 480;
		props.imguiProps.isDockingEnable = true;

		return props;
	}
	void initialize() override
	{
		auto& window=Engine::Instance().getWindow();
		window.setShouldRenderToScreen(false);

		data = worker.exec("Select 'Output';");
		tables = worker.exec("Select table_name from information_schema.tables where table_schema = 'public' order by table_name asc; ");
		numOfRows = data.size();
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 16);

			//  executeQuery(worker, "DELETE FROM \"User\" WHERE user_name='himanshu';");
			//  executeQuery(worker,"SELECT Register('himanshu',20,9876543310)");
			//executeQuery(worker,"SELECT GenerateTournament(205,'xyz tournament',10000,305,'20-12-2021');");
		    //  executeQuery(worker,"SELECT * FROM \"User\";");
			 // executeQuery(worker,"SELECT * FROM Tournament;");

			 //Find details of staff who have solved at least one complaintand has salary less than average.
			 // executeQuery(worker, "SELECT DISTINCT staff.staff_id, staff.name, staff.age, staff.contact, staff.salary FROM staff INNER JOIN complaints ON staff.staff_id = complaints.staff_id WHERE status = 'Resolved' AND salary < (SELECT AVG(salary) FROM staff); ");

			  //List all users who have won more than one tournament.
			  //executeQuery(worker, "SELECT user_name FROM \"User\" INNER JOIN Tournament ON User_ID = First_winner OR User_ID = Second_winner OR User_ID = Third_winner GROUP BY user_name HAVING COUNT(user_name) > 1;");

			  //Find the average price of all paid games.
			  //executeQuery(worker, "SELECT AVG(price) FROM games_price WHERE price != 0; ");


			
	}

	void shutdown() override
	{
	}

	void update(float deltaTime)   override
	{
		deltaTime;
	}

	void render()   override
	{
		
	}

	void imguiRender()  override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		

		if (ImGui::Begin("Commands"))
		{
			if (ImGui::TreeNode("Tables"))
			{
				for (auto row : tables)
				{
					for (auto e : row)
					{
						std::string name = std::string(e.c_str()) + " Table";
						std::string query = "SELECT * FROM \"" + std::string(e.c_str()) + "\";";
						if (ImGui::Button(name.c_str()))
						{
							data = worker.exec(query.c_str());
							numOfRows = data.size();
						}
					}
				}
				ImGui::TreePop();
			}

			ImGui::Text("Register User");
			if(ImGui::InputTextWithHint("User Name","Name",rbuffer1,120))
			{
				rname = rbuffer1;
			}
			ImGui::InputInt("age", &rage);
			if (ImGui::InputTextWithHint("contact", "9876543210", rbuffer2, 120))
			{
				rcnumber = rbuffer2;
			}
			if (ImGui::Button("Register"))
			{
				std::string query = "SELECT Register('" + rname + "'," + std::to_string(rage) + "," + rcnumber + ");";
				data = worker.exec(query.c_str());
				numOfRows = data.size();
			}

			ImGui::Text("Delete User");
			ImGui::InputInt("ID", &dID);
			if (ImGui::Button("Delete"))
			{
				 std::string query = "DELETE FROM \"User\" WHERE user_ID =" + std::to_string(dID) + ";";
				 worker.exec(query.c_str());
				 data = worker.exec("SELECT 'Deleted User with ID = 187';");
				 numOfRows = data.size();
			}

			ImGui::Text("Generate Tournament");
			ImGui::InputInt("Game ID", &tgid);
			if (ImGui::InputTextWithHint("Name", "xyz tournament", tbuffer1, 120))
			{
				tname = tbuffer1;
			}
			ImGui::InputInt("Reward money", &trewards);
			ImGui::InputInt("Staff ID", &tsid);
			if (ImGui::InputTextWithHint("Date", "DD-MM-YYYY", tbuffer2, 120))
			{
				tdate = tbuffer2;
			}
			if (ImGui::Button("Generate"))
			{
				std::string query = "SELECT GenerateTournament(" + std::to_string(tgid) + ",'" + tname + "'," + std::to_string(trewards) + "," + std::to_string(tsid) + ",'" + tdate + "');";
				std::cout << query;
				data = worker.exec(query.c_str());
				numOfRows = data.size();
			}

			if (ImGui::Button("Commit changes"))
			{
				worker.commit();
			}
		}	
		ImGui::End();
		
		if (ImGui::Begin("Queries"))
		{
			ImGui::Text(eq1.c_str());
			if (ImGui::Button("execute Query - 1"))
			{
				data = worker.exec("SELECT DISTINCT staff.staff_id, staff.name, staff.age, staff.contact, staff.salary FROM staff INNER JOIN complaints ON staff.staff_id = complaints.staff_id WHERE status = 'Resolved' AND salary < (SELECT AVG(salary) FROM staff);");
				numOfRows = data.size();
			}
			ImGui::Text(eq2.c_str());
			if (ImGui::Button("execute Query - 2"))
			{
				data = worker.exec("SELECT user_name FROM \"User\" INNER JOIN Tournament ON User_ID = First_winner OR User_ID = Second_winner OR User_ID = Third_winner GROUP BY user_name HAVING COUNT(user_name) > 1;");
				numOfRows = data.size();
			}
			ImGui::Text(eq3.c_str());
			if (ImGui::Button("execute Query - 3"))
			{
				data = worker.exec("SELECT AVG(price) FROM games_price WHERE price != 0; ");
				numOfRows = data.size();
			}
		}
		ImGui::End();

		if (ImGui::Begin("Results"))
		{
			ImGui::SliderInt("number of display rows", &numOfRows,0, data.size());
			
			ImGui::BeginTable("tab", data[0].size());
			for (int row=0;row<numOfRows;row++)
			{
				ImGui::TableNextRow();
				for(int i=0;i<data[0].size();i++)
				{
					ImGui::TableSetColumnIndex(i);
					ImGui::Text(data[row][i].c_str());
				}
			}
			ImGui::EndTable();
		}
		ImGui::End();
		
	}
};

banana::App* createApp()
{
	return new Editor();
}