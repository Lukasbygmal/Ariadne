#include "auth.h"
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <thread>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif

using json = nlohmann::json;

namespace auth
{
    void startGithubLogin()
    {
        // Open the GitHub OAuth login URL in the user's browser
        std::string url = "http://localhost:5000/login/github";
        system("cmd.exe /c start firefox \"-private-window\" http://localhost:5000/login/github");
    }

    bool exchangeCodeForUserId(const std::string &code, int &user_id)
    {
        // Send the code/token to the backend to exchange for user_id
        httplib::Client cli("localhost", 5000);
        json req_body = {{"code", code}};
        auto res = cli.Post("/api/oauth/exchange", req_body.dump(), "application/json");
        if (res && res->status == 200)
        {
            auto resp_json = json::parse(res->body);
            if (resp_json.contains("user_id"))
            {
                user_id = resp_json["user_id"];
                return true;
            }
        }
        return false;
    }

    bool pollForUserId(int &user_id, int max_attempts, int delay_ms)
    {
        httplib::Client cli("localhost", 5000);
        for (int attempt = 0; attempt < max_attempts; ++attempt)
        {
            auto res = cli.Get("/api/session");
            if (res && res->status == 200)
            {
                auto resp_json = json::parse(res->body);
                if (resp_json.contains("logged_in") && resp_json["logged_in"] == true)
                {
                    user_id = resp_json["user_id"];
                    return true;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        }
        return false;
    }
}
