#include <node_api.h>

#include <cstdint>
#include <cstring>
#include <string>

extern "C" bool initializeRenderer(void* parentView, const char* projectRoot);
extern "C" void setRendererViewport(int x, int y, int width, int height);
extern "C" bool rendererShouldClose();
extern "C" void renderFrame();
extern "C" void shutdownRenderer();

namespace {
napi_value throwError(napi_env env, const char* message) {
    napi_throw_error(env, nullptr, message);
    return nullptr;
}

napi_value start(napi_env env, napi_callback_info info) {
    size_t argc = 2;
    napi_value args[2];
    if (napi_get_cb_info(env, info, &argc, args, nullptr, nullptr) != napi_ok || argc != 2) {
        return throwError(env, "start requires the Electron native window handle and project path.");
    }

    void* data = nullptr;
    size_t length = 0;
    if (napi_get_buffer_info(env, args[0], &data, &length) != napi_ok ||
        length < sizeof(uintptr_t)) {
        return throwError(env, "The Electron native window handle is invalid.");
    }

    uintptr_t handle = 0;
    std::memcpy(&handle, data, sizeof(handle));
    size_t rootLength = 0;
    if (napi_get_value_string_utf8(env, args[1], nullptr, 0, &rootLength) != napi_ok) {
        return throwError(env, "The project path is invalid.");
    }
    std::string projectRoot(rootLength + 1, '\0');
    size_t copiedLength = 0;
    if (napi_get_value_string_utf8(
            env, args[1], projectRoot.data(), projectRoot.size(), &copiedLength
        ) != napi_ok) {
        return throwError(env, "The project path is invalid.");
    }
    projectRoot.resize(copiedLength);

    if (!initializeRenderer(reinterpret_cast<void*>(handle), projectRoot.c_str())) {
        return throwError(env, "Failed to initialize the OpenGL renderer.");
    }

    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value setViewport(napi_env env, napi_callback_info info) {
    size_t argc = 4;
    napi_value args[4];
    if (napi_get_cb_info(env, info, &argc, args, nullptr, nullptr) != napi_ok || argc != 4) {
        return throwError(env, "setViewport requires x, y, width, and height.");
    }

    int32_t bounds[4];
    for (size_t i = 0; i < 4; ++i) {
        if (napi_get_value_int32(env, args[i], &bounds[i]) != napi_ok) {
            return throwError(env, "Viewport bounds must be integers.");
        }
    }

    setRendererViewport(bounds[0], bounds[1], bounds[2], bounds[3]);
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value render(napi_env env, napi_callback_info) {
    renderFrame();
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value shouldClose(napi_env env, napi_callback_info) {
    napi_value result;
    napi_get_boolean(env, rendererShouldClose(), &result);
    return result;
}

napi_value stop(napi_env env, napi_callback_info) {
    shutdownRenderer();
    napi_value result;
    napi_get_undefined(env, &result);
    return result;
}

napi_value initializeAddon(napi_env env, napi_value exports) {
    const napi_property_descriptor properties[] = {
        {"start", nullptr, start, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"setViewport", nullptr, setViewport, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"render", nullptr, render, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"shouldClose", nullptr, shouldClose, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"stop", nullptr, stop, nullptr, nullptr, nullptr, napi_default, nullptr}
    };
    if (napi_define_properties(env, exports, sizeof(properties) / sizeof(properties[0]), properties) != napi_ok) {
        return throwError(env, "Failed to initialize the OpenGL addon.");
    }
    return exports;
}
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, initializeAddon)
