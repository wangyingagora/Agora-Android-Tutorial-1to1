//
// Created by eaglewangy on 2018/9/2.
//

#include <jni.h>
#include <memory>
#include <thread>
#include <string>
#include <android/log.h>
#include "agora/include/IAgoraRtcEngine.h"
#include "agora/include/AgoraBase.h"
#include "agora/include/IAgoraMediaEngine.h"

#ifndef AGORACPPDEMO_AGORAENGINE_H
#define AGORACPPDEMO_AGORAENGINE_H

static const char* TAG = "cpp";

class AgoraHandler;

class AgoraEngine
{
public:
    AgoraEngine(JNIEnv* env, jobject context, bool async);
    void createEngine(JNIEnv* env, jobject context);
    int setupLocalVideo(JNIEnv* env, jobject thiz);
    int createRemoteVideo(uid_t uid);
    int setupRemoteVideo(JNIEnv* env, jobject thiz, uid_t uid);
    int joinChannel(const char* channelId);

    ~AgoraEngine();
private:
    bool mAsync;
    std::unique_ptr<std::thread> mThread;
    std::unique_ptr<agora::rtc::IRtcEngine> mEngine;
    std::unique_ptr<AgoraHandler> mHandler;
    jobject mJavaActivity;
    jmethodID mCreateRemoteViewMethodId;
};

class AgoraHandler : public agora::rtc::IRtcEngineEventHandler
{
public:
    AgoraHandler(AgoraEngine& engine) :
            mEngine(engine) {
    }

    virtual void onJoinChannelSuccess(const char* channel, uid_t userId, int elapsed) override {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "joined channel: %s, uid: %u", channel, userId);
    }

    virtual void onUserJoined(uid_t userId, int elapsed) override {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "joined user: %u", userId);
    }

    virtual void onFirstRemoteVideoDecoded(uid_t userId, int width, int height, int elapsed) {
        mEngine.createRemoteVideo(userId);
    }

private:
    AgoraEngine& mEngine;
};

#endif //AGORACPPDEMO_AGORAENGINE_H
