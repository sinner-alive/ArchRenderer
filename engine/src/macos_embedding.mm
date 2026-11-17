#import <Cocoa/Cocoa.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

namespace {
NSView* parentView = nil;
NSWindow* embeddedWindow = nil;

void setEmbeddedWindowFrame(int x, int y, int width, int height) {
    NSWindow* parent = parentView.window;
    if (!parent || !embeddedWindow) {
        return;
    }

    NSRect viewportInView = NSMakeRect(
        x,
        parentView.bounds.size.height - y - height,
        width,
        height
    );
    NSRect viewportInWindow = [parentView convertRect:viewportInView toView:nil];
    NSRect viewportOnScreen = [parent convertRectToScreen:viewportInWindow];
    [embeddedWindow setFrame:viewportOnScreen display:YES];
    [embeddedWindow orderFront:nil];
}
}

extern "C" bool attachGLFWWindowToView(GLFWwindow* window, void* nativeParentView) {
    parentView = (NSView*)nativeParentView;
    NSWindow* parent = parentView.window;
    embeddedWindow = glfwGetCocoaWindow(window);
    if (!parent || !embeddedWindow) {
        return false;
    }

    [embeddedWindow setHasShadow:NO];
    [parent addChildWindow:embeddedWindow ordered:NSWindowAbove];
    return true;
}

extern "C" void resizeEmbeddedGLFWWindow(GLFWwindow* window, int x, int y, int width, int height) {
    (void)window;
    setEmbeddedWindowFrame(x, y, width, height);
}
