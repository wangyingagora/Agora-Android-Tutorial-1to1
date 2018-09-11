//
// Created by eaglewangy on 2018/9/2.
//

#include <jni.h>
#include <memory>
#include <thread>
#include <string>
#include <sstream>
#include <vector>
#include <android/log.h>
#include "agora/include/IAgoraRtcEngine.h"
#include "agora/include/AgoraBase.h"
#include "agora/include/IAgoraMediaEngine.h"

#ifndef AGORACPPDEMO_AGORAENGINE_H
#define AGORACPPDEMO_AGORAENGINE_H

static const char* TAG = "cpp";

class Packer {
public:
    Packer() :
     lenght(2)
    , position(2) {
    }

    void writeInt32(uint32_t val) {
        memcpy(&buf[0] + position, &val, sizeof(val));
        position += sizeof(val);
        lenght += sizeof(val);
    }

    short length() {
        return lenght;
    }

    const char* buffer() { return  &buf[0]; }
private:
    char buf[512];
    int lenght;
    int position;
};

class AgoraHandler;

class AgoraEngine
{
public:
    AgoraEngine(JNIEnv* env, jobject context, bool async);
    void createEngine(JNIEnv* env, jobject context);
    int setupLocalVideo(JNIEnv* env, jobject thiz, jobject view);
    int createRemoteVideo(uid_t uid);
    int setupRemoteVideo(JNIEnv* env, jobject thiz, uid_t uid, jobject view);
    int joinChannel(const char* channelId);
    int leaveChannel();
    int switchCamera();
    int muteLocalAudioStream(bool mute);
    int muteLocalVideoStream(bool mute);

    int onMessage(int messageId, std::string* payload);

    ~AgoraEngine();
private:
    bool mAsync;
    std::unique_ptr<std::thread> mThread;
    std::unique_ptr<agora::rtc::IRtcEngine> mEngine;
    std::unique_ptr<AgoraHandler> mHandler;
    jobject mJavaActivity;
    jmethodID mCreateRemoteViewMethodId;
    jmethodID mOnMessageMethodId;
};

class AgoraHandler : public agora::rtc::IRtcEngineEventHandler
{
public:
    AgoraHandler(AgoraEngine& engine) :
            mEngine(engine) {
    }

    virtual void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed) override {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "joined channel: %s, uid: %u", channel, uid);

        short position = 2;
        char buf[512];
        memcpy(&buf[0] + position, &uid, sizeof(uid));
        position += sizeof(uid);
        memcpy(&buf[0] + position, channel, strlen(channel));
        position += strlen(channel);
        memcpy(&buf[0], &position, sizeof(position));
        std::string payload(&buf[0], position);

        mEngine.onMessage(1, &payload);
    }

    virtual void onUserJoined(uid_t uid, int elapsed) override {
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "joined user: %u", uid);
        std::ostringstream oss;
        oss << uid;
        std::string payload(oss.str().c_str(), oss.str().length());
        mEngine.onMessage(2, &payload);
    }

    virtual void onUserOffline(uid_t uid, agora::rtc::USER_OFFLINE_REASON_TYPE reason) {
        std::ostringstream oss;
        oss << uid << reason;
        std::string payload(oss.str().c_str(), oss.str().length());
        mEngine.onMessage(3, &payload);
    }

    virtual void onUserMuteVideo(uid_t uid, bool muted) {
        std::ostringstream oss;
        oss << uid << muted;
        std::string payload(oss.str().c_str(), oss.str().length());
        mEngine.onMessage(4, &payload);
    }

    virtual void onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed) {
        /*
        int position = 0;
        std::vector<char> v;
        v.resize(sizeof(uid));
        position += sizeof(uid);
        memcpy(&v[0], &uid, sizeof(uid));
        std::string payload(&v[0], position);
        mEngine.onMessage(5, &payload);
        */

        __android_log_print(ANDROID_LOG_DEBUG, TAG, "c++1 uid: %u, len: %d", uid);
        mEngine.createRemoteVideo(uid);
    }

private:
    AgoraEngine& mEngine;
};

#endif //AGORACPPDEMO_AGORAENGINE_H
