#include "auth.hpp"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <windows.h>
#include <shellapi.h>

using json = nlohmann::json;

namespace auth
{
    void startGithubLogin()
    {
        // Open the GitHub OAuth login URL in the user's browser
        std::string url = "https://ariadne-t99a.onrender.com/login/github";
        ShellExecuteA(
            NULL,         // Parent window handle
            "open",       // Action to perform
            url.c_str(),  // URL to open
            NULL,         // Parameters (none needed)
            NULL,         // Working directory (use default)
            SW_SHOWNORMAL // Show command
        );
    }

    bool exchangeCodeForUserId(const std::string &code, int &user_id)
    {
        httplib::SSLClient cli("ariadne-t99a.onrender.com", 443);
        cli.set_follow_location(true);
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
        httplib::SSLClient cli("ariadne-t99a.onrender.com", 443);
        cli.set_follow_location(true);
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
