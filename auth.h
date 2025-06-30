#pragma once
#include <string>

namespace auth {
    void startGithubLogin();
    bool exchangeCodeForUserId(const std::string &code, int &user_id);
    bool pollForUserId(int &user_id, int max_attempts = 30, int delay_ms = 1000);
}