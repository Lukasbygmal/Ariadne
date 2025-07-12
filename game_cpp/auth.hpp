/**
 * @file auth.hpp
 * @brief GitHub OAuth authentication functionality for desktop applications
 *
 * This module provides OAuth authentication with GitHub through a local web server.
 * It handles opening the browser for login, exchanging authorization codes for user IDs,
 * and polling for authentication completion.
 */
#pragma once
#include <string>

/**
 * @namespace auth
 * @brief Authentication utilities for GitHub OAuth integration
 *
 * Provides functions to initiate GitHub OAuth login flow, exchange authorization
 * codes for user credentials, and poll for authentication status.
 */
namespace auth
{
    /**
     * @brief Initiates GitHub OAuth login by opening browser to authentication URL
     *
     * Opens browser to the GitHub OAuth login endpoint.
     * This starts the OAuth flow for user authentication.
     *
     * @note Support for only windows
     */
    void startGithubLogin();

    /**
     * @brief Exchanges OAuth authorization code for authenticated user ID
     *
     * Sends the authorization code received from GitHub OAuth callback to the
     * backend server to exchange for a user ID. This completes the OAuth flow.
     *
     * @param code OAuth authorization code from GitHub callback
     * @param user_id Reference to store the retrieved user ID
     * @return true if exchange successful and user_id populated, false otherwise
     */
    bool exchangeCodeForUserId(const std::string &code, int &user_id);

    /**
     * @brief Polls backend server for user authentication status
     *
     * Repeatedly checks the session endpoint to determine if user has completed
     * authentication. Useful for detecting when OAuth flow completes asynchronously.
     *
     * @param user_id Reference to store the authenticated user ID when found
     * @param max_attempts Maximum number of polling attempts (default: 30)
     * @param delay_ms Delay between polling attempts in milliseconds (default: 4000)
     * @return true if user authenticated within attempt limit, false if timed out
     */
    bool pollForUserId(int &user_id, int max_attempts = 30, int delay_ms = 4000);
}