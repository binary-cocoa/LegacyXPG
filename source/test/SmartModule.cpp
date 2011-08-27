#include "SmartModule.hpp"

SmartModule::SmartModule()
{
}

SmartModule::~SmartModule()
{
}

void SmartModule::handleEvent(const XPG::Event& inEvent)
{
    switch (inEvent.type)
    {
        case XPG::Event::Window:
        {
            switch (inEvent.window.event)
            {
                case XPG::WindowEvent::Focus:
                {
                    onFocus();
                    break;
                }

                case XPG::WindowEvent::Blur:
                {
                    onBlur();
                    break;
                }

                case XPG::WindowEvent::Resize:
                {
                    onResize(inEvent.window.width, inEvent.window.height);
                    break;
                }

                case XPG::WindowEvent::Exit:
                {
                    onExit();
                    break;
                }

                default: {}
            }

            break;
        }

        case XPG::Event::Keyboard:
        {
            switch (inEvent.keyboard.event)
            {
                case XPG::KeyboardEvent::Press:
                {
                    onKeyDown(inEvent.keyboard.key);
                    break;
                }

                case XPG::KeyboardEvent::Release:
                {
                    onKeyUp(inEvent.keyboard.key);
                    break;
                }

                case XPG::KeyboardEvent::Repeat:
                {
                    onKeyRepeat(inEvent.keyboard.key);
                    break;
                }

                default: {}
            }

            break;
        }

        case XPG::Event::Mouse:
        {
            switch (inEvent.mouse.event)
            {
                case XPG::MouseEvent::Motion:
                {
                    onMouseMove(inEvent.mouse.x, inEvent.mouse.y);
                    break;
                }

                case XPG::MouseEvent::ButtonDown:
                {
                    switch (inEvent.mouse.button)
                    {
                        case XPG::MouseEvent::LeftButton:
                        {
                            onLeftButtonDown();
                            break;
                        }

                        case XPG::MouseEvent::RightButton:
                        {
                            onRightButtonDown();
                            break;
                        }

                        case XPG::MouseEvent::MiddleButton:
                        {
                            onMiddleButtonDown();
                            break;
                        }

                        default: {}
                    }

                    break;
                }

                case XPG::MouseEvent::ButtonUp:
                {
                    switch (inEvent.mouse.button)
                    {
                        case XPG::MouseEvent::LeftButton:
                        {
                            onLeftButtonUp();
                            break;
                        }

                        case XPG::MouseEvent::RightButton:
                        {
                            onRightButtonUp();
                            break;
                        }

                        case XPG::MouseEvent::MiddleButton:
                        {
                            onMiddleButtonUp();
                            break;
                        }

                        default: {}
                    }

                    break;
                }

                case XPG::MouseEvent::WheelDown:
                {
                    onMouseWheel(false, true);
                    break;
                }

                case XPG::MouseEvent::WheelUp:
                {
                    onMouseWheel(true, false);
                    break;
                }

                case XPG::MouseEvent::LeaveWindow:
                {
                    onMouseLeave();
                    break;
                }

                case XPG::MouseEvent::EnterWindow:
                {
                    onMouseEnter();
                    break;
                }

                default: {}
            }
            break;
        }

        default: {}
    }
}

void SmartModule::onKeyDown(XPG::Key::Code inKey)
{
    if (inKey == XPG::Key::Escape) stopRunning();
}

void SmartModule::onKeyUp(XPG::Key::Code inKey) {}
void SmartModule::onKeyRepeat(XPG::Key::Code inKey) {}

void SmartModule::onMouseMove(uint32 inX, uint32 inY) {}
void SmartModule::onLeftButtonDown() {}
void SmartModule::onLeftButtonUp() {}
void SmartModule::onRightButtonDown() {}
void SmartModule::onRightButtonUp() {}
void SmartModule::onMiddleButtonDown() {}
void SmartModule::onMiddleButtonUp() {}
void SmartModule::onMouseWheel(bool inUp, bool inDown) {}
void SmartModule::onMouseLeave() {}
void SmartModule::onMouseEnter() {}

void SmartModule::onFocus() {}
void SmartModule::onBlur() {}
void SmartModule::onResize(uint32 inWidth, uint32 inHeight) {}

void SmartModule::onExit()
{
    stopRunning();
}
