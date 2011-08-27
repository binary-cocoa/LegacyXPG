#ifndef SMARTMODULE_HPP
#define SMARTMODULE_HPP

#include <XPG/Module.hpp>
#include "LiberateTypes.hpp"

class SmartModule : public XPG::Module
{
    public:
        SmartModule();
        virtual ~SmartModule();

        virtual void handleEvent(const XPG::Event& inEvent);

        virtual void onKeyDown(XPG::Key::Code inKey);
        virtual void onKeyUp(XPG::Key::Code inKey);
        virtual void onKeyRepeat(XPG::Key::Code inKey);

        virtual void onMouseMove(uint32 inX, uint32 inY);
        virtual void onLeftButtonDown();
        virtual void onLeftButtonUp();
        virtual void onRightButtonDown();
        virtual void onRightButtonUp();
        virtual void onMiddleButtonDown();
        virtual void onMiddleButtonUp();
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onMouseLeave();
        virtual void onMouseEnter();

        virtual void onFocus();
        virtual void onBlur();
        virtual void onResize(uint32 inWidth, uint32 inHeight);
        virtual void onExit();

    private:
};

#endif
