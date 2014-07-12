// ASTEROIDS_LICENSE_BEGIN
/*

  asteroids GPL Source Code
  Copyright (C) 2013 Edward Knyshov

  This file is part of the asteroids GPL Source Code (asteroids Source Code).

  asteroids Source Code is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  asteroids Source Code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with asteroids Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// ASTEROIDS_LICENSE_END

/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "App.hpp"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

// Our application

test::App application;

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 */
struct engine {
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
	application.ScreenSize(w, h);
    engine->state.angle = 0;

    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine) {
    if (engine->display == NULL) {
        // No display.
        return;
    }

    // Just fill the screen with a color.
    glClearColor(0xe1 / 255.0f, 0xfa / 255.0f, 0xfc / 255.0f, 1);
	glColor4f(0xe4 / 255.0f, 0x56 / 255.0f, 0x35 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2.1f);

	application.Draw();

    eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animating = 1;

		auto count = AMotionEvent_getPointerCount(event);
		int id0 = -1;
		int id1 = -1;

		id0 = AMotionEvent_getPointerId(event, 0);
		if (count > 1) {
		  id1 = AMotionEvent_getPointerId(event, 1);
		}

		static int playerslot0 = -1;
		static int playerslot1 = -1;
		static int playerslot0index = -1;
		static int playerslot1index = -1;

		auto action = AKeyEvent_getAction(event);
		auto getX = [&] (int index) {
		  return AMotionEvent_getX(event, index);
		};
		auto getY = [&] (int index) {
		  return AMotionEvent_getY(event, index);
		};

		switch (action & AMOTION_EVENT_ACTION_MASK) {
		case AMOTION_EVENT_ACTION_DOWN:
		  break;
		case AMOTION_EVENT_ACTION_POINTER_DOWN:
		  break;
		case AMOTION_EVENT_ACTION_MOVE:
		  if (playerslot0 == -1) {
			if (playerslot1 == id0) {
			  if (count > 1) {
				playerslot0 = id1;
				playerslot0index = 1;
				application.Touch(0,
								  getX(playerslot0index),
								  getY(playerslot0index));
			  }
			}
			else {
			  playerslot0 = id0;
			  playerslot0index = 0;
			  application.Touch(0,
								getX(playerslot0index),
								getY(playerslot0index));
			}
		  }
		  else {
			if (playerslot0 == id0) {
			  playerslot0index = 0;
			  application.Touch(0,
								getX(playerslot0index),
								getY(playerslot0index));
			}
			else if (playerslot0 == id1 && count > 1) {
			  playerslot0index = 1;
			  application.Touch(0,
								getX(playerslot0index),
								getY(playerslot0index));
			}
		  }

		  if (playerslot1 == -1) {
			if (playerslot0 == id0) {
			  if (count > 1) {
				playerslot1index = 1;
				playerslot1 = id1;
				application.Touch(1,
								  getX(playerslot1index),
								  getY(playerslot1index));
			  }
			}
			else {
			  playerslot1index = 0;
			  playerslot1 = id0;
			  application.Touch(1,
								getX(playerslot1index),
								getY(playerslot1index));
			}
		  }
		  else {
			if (playerslot1 == id0) {
			  playerslot1index = 0;
			  application.Touch(1,
								getX(playerslot1index),
								getY(playerslot1index));
			}
			else if (playerslot1 == id1 && count > 1) {
			  playerslot1index = 1;
			  application.Touch(1,
								getX(playerslot1index),
								getY(playerslot1index));
			}
		  }

		  // if (count == 1) {
		  // 	if (playerslot0 != -1 && playerslot1 == -1) {
		  // 	  nearest = application.NearestPlayer(getX(playerslot0index),
		  // 										  getY(playerslot0index));

		  // 	  if (nearest == 1) {
		  // 		std::swap(playerslot0, playerslot1);
		  // 		std::swap(playerslot0index, playerslot1index);
		  // 	  }
		  // 	}
		  // 	else if (playerslot1 != -1 && playerslot0 == -1) {
		  // 	  if (playerslot0 != -1 && playerslot1 == -1) {
		  // 		nearest = application.NearestPlayer(getX(playerslot0index),
		  // 											getY(playerslot0index));

		  // 		if (nearest == 1) {
		  // 		  std::swap(playerslot0, playerslot1);
		  // 		  std::swap(playerslot0index, playerslot1index);
		  // 		}
		  // 	  }
		  // 	}
		  // }
		  // else if (count == 2) {
		  // }

		  break;
		case AMOTION_EVENT_ACTION_UP:
		  if (id0 != -1) {
		  	if (id0 == playerslot0) {
		  	  playerslot0 = -1;
			  playerslot0index = -1;
		  	  application.TouchEnd(0, 0, 0);
		  	}
		  	else if (id0 == playerslot1) {
		  	  playerslot1 = -1;
			  playerslot1index = -1;
		  	  application.TouchEnd(1, 0, 0);
		  	}
		  }

		  if (count > 1 && id1 != -1) {
		  	if (id1 == playerslot0) {
		  	  playerslot0 = -1;
			  playerslot0index = -1;
		  	  application.TouchEnd(0, 0, 0);
		  	}
		  	else if (id1 == playerslot1) {
		  	  playerslot1 = -1;
			  playerslot1index = -1;
		  	  application.TouchEnd(1, 0, 0);
		  	}
		  }
		  break;
		case AMOTION_EVENT_ACTION_CANCEL:
		  break;
		case AMOTION_EVENT_ACTION_POINTER_UP:
		  break;
		default:
		  break;
		}


        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
		  engine_term_display(engine);
		  application.Release();
		  break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    srand((time(NULL)));

    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }

	// init application

	application.Init();
	struct timeval prevTime;
	struct timeval curTime = prevTime;
	prevTime.tv_sec = 0;
	prevTime.tv_usec = 0;
	gettimeofday(&prevTime, NULL);

    // loop waiting for stuff to do.

    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) {
                        LOGI("accelerometer: x=%f y=%f z=%f",
                                event.acceleration.x, event.acceleration.y,
                                event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) {
                engine.state.angle = 0;
            }

			gettimeofday(&curTime, NULL);
			application.Update(((double)curTime.tv_sec + (double)curTime.tv_usec / 1000000) -
							   ((double)prevTime.tv_sec + (double)prevTime.tv_usec / 1000000.0));
			prevTime = curTime;

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
//END_INCLUDE(all)
