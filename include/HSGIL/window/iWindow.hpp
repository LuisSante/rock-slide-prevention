/********************************************************************************
 *                                                                              *
 * HSGIL - Handy Scalable Graphics Integration Library                          *
 * Copyright (c) 2019-2022 Adrian Bedregal                                      *
 *                                                                              *
 * This software is provided 'as-is', without any express or implied            *
 * warranty. In no event will the authors be held liable for any damages        *
 * arising from the use of this software.                                       *
 *                                                                              *
 * Permission is granted to anyone to use this software for any purpose,        *
 * including commercial applications, and to alter it and redistribute it       *
 * freely, subject to the following restrictions:                               *
 *                                                                              *
 * 1. The origin of this software must not be misrepresented; you must not      *
 *    claim that you wrote the original software. If you use this software      *
 *    in a product, an acknowledgment in the product documentation would be     *
 *    appreciated but is not required.                                          *
 * 2. Altered source versions must be plainly marked as such, and must not be   *
 *    misrepresented as being the original software.                            *
 * 3. This notice may not be removed or altered from any source distribution.   *
 *                                                                              *
 ********************************************************************************/

#ifndef HSGIL_I_WINDOW_HPP
#define HSGIL_I_WINDOW_HPP

#include <HSGIL/config/config.hpp>
#include <HSGIL/config/common.hpp>

#include <HSGIL/window/inputHandler.hpp>

namespace gil
{
class WindowManager;

/**
 * @brief Window Class that handle a Window of the program
 * 
 */
class HSGIL_API IWindow
{
public:
    /**
     * @brief Construct a new IWindow object
     * 
     * @param t_width 
     * @param t_height 
     * @param t_title 
     * @param t_eventHandler 
     */
    IWindow(const int t_width, const int t_height, const char* t_title, InputHandler* t_inputHandler) : m_windowWidth {t_width}, m_windowHeight {t_height}, m_title {t_title}, m_ready {false}, m_inputHandler {t_inputHandler} {}
    /**
     * @brief Destroy the Window object
     * 
     */
    virtual ~IWindow() {}

    /**
     * @brief Check if the Window shouldn't close
     * 
     * @return true if the Window is active
     * @return false if the Windows is not active
     */
    virtual bool isActive() = 0;
    /**
     * @brief Check if the Window is able to start rendering
     * 
     * @return true if the Window is able
     * @return false if the Window is not able
     */
    virtual bool isReady() = 0;
    /**
     * @brief Send signal to close window
     * 
     */
    virtual void close() = 0;

    /**
     * @brief Set the Input Handler object
     * 
     */
    virtual void setInputHandler(InputHandler& t_inputHandler) = 0;
    /**
     * @brief Poll the Events to process the input
     * 
     */
    virtual void pollEvents() = 0;
    /**
     * @brief Get the Aspect Ratio
     * 
     * @return float value containing the current aspect ratio
     */
    virtual float getAspectRatio() const = 0;
    /**
     * @brief Get the Window Rect object as Vec2i
     * 
     * @return Vec2i vector containing width and height of the window in pixels
     */
    virtual Vec2i getWindowRect() const = 0;
    /**
     * @brief Get the Workspace Rect object as Vec2i
     * 
     * @return Vec2i vector containing width and height of the viewport in pixels
     */
    virtual Vec2i getViewportRect() const = 0;

protected:
    /**
     * @brief Initializes the Window itself
     * 
     */
    virtual void initializeWindow() = 0;

    int m_windowWidth;
    int m_windowHeight;
    int m_viewportWidth;
    int m_viewportHeight;
    const char* m_title;

    bool m_ready;

    InputHandler* m_inputHandler;
    WindowManager* m_windowManager;
};

} // namespace gil

#endif // HSGIL_I_WINDOW_HPP