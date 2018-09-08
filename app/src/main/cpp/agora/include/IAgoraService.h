//
//  Agora SDK
//
//  Created by Sting Feng in 2017-11.
//  Copyright (c) 2017 Agora.io. All rights reserved.
//

#pragma once  // NOLINT(build/header_guard)

#include "AgoraBase.h"

namespace agora {
namespace api {
class IRtcConnection;
class IPublisher;
class ISubscriber;
class IMediaDeviceManager;
struct TConnectionInfo;
struct RtcConnectionConfiguration;
}  // namespace api
namespace rtc {
class IRtcEngine;
}
namespace signaling {
class ISignalingEngine;
}
namespace base {
class IEngineBase;

/** Enumerates service profile, which preset configurations of Media Engine
automatically. */
enum TServiceProfile {
  /* The default setting. It is most commonly used in voice and video call,
      where you can talk freely and have a video chat.
  */
  PROFILE_COMMUNICATION,
  /* Live Broadcast has host and audience roles. You can send Audio and Video
      media stream as host anytime, and if there is audience host-in, you may
     also receive meida stream.
  */
  PROFILE_LIVE_BROADCASTING,
  /* Game Voice Mode. you can speak freely. This mode uses the codec with low
      power consumption and low bitrate by default.
  */
  PROFILE_GAME
};

struct AgoraServiceConfiguration {
  TServiceProfile serviceProfile;
  bool enableAudio;
  bool enableVideo;
};

class IAgoraService {
 public:
  virtual void release() = 0;
  /**
   * initialize the Agora Service
   * @param [in] context
   *        the Agora Service context
   * @return return 0 if success or an error code
   */
  virtual int initialize(const AgoraServiceConfiguration& context) = 0;

  /**
   * get the version information of the SDK
   * @param [in, out] build
   *        the build number
   * @return return the version number string in char format
   */
  virtual const char* getVersion(int* build) = 0;

  virtual int setLogFile(const char* filePath, unsigned int fileSize) = 0;
  virtual int setLogFilter(unsigned int filters) = 0;

  virtual signaling::ISignalingEngine* createSignalingEngine() = 0;

  virtual api::IRtcConnection* createRtcConnection(
      const api::RtcConnectionConfiguration& cfg) = 0;

  /** This method enumerates current connections.
   * Deprecated this method for now
   */
  //virtual void enumerateConnections(util::AList<api::TConnectionInfo>& connections) = 0;

  // Return Conenction handler by the connection identifier, return NULL if
  // connectionId is invalid.
  virtual api::IRtcConnection* getRtcConnection(const TConnectionId
	  connectionId) = 0;

  virtual api::IPublisher* createPublisher() = 0;
  virtual api::ISubscriber* createSubscriber() = 0;
  virtual api::IMediaDeviceManager* createMediaDeviceManager(
      IEngineBase& engine) = 0;

	virtual ~IAgoraService() {}
};

class IAgoraParameter {
 public:
  /**
   * release the resource
   */
  virtual void release() = 0;

  /**
   * set bool value of the json
   * @param [in] key
   *        the key name
   * @param [in] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int setBool(const char* key, bool value) = 0;

  /**
   * set int value of the json
   * @param [in] key
   *        the key name
   * @param [in] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int setInt(const char* key, int value) = 0;

  /**
   * set unsigned int value of the json
   * @param [in] key
   *        the key name
   * @param [in] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int setUInt(const char* key, unsigned int value) = 0;

  /**
   * set double value of the json
   * @param [in] key
   *        the key name
   * @param [in] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int setNumber(const char* key, double value) = 0;

  /**
   * set string value of the json
   * @param [in] key
   *        the key name
   * @param [in] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int setString(const char* key, const char* value) = 0;

  /**
   * set object value of the json
   * @param [in] key
   *        the key name
   * @param [in] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int setObject(const char* key, const char* value) = 0;

  /**
   * get bool value of the json
   * @param [in] key
   *        the key name
   * @param [in, out] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int getBool(const char* key, bool& value) = 0;

  /**
   * get int value of the json
   * @param [in] key
   *        the key name
   * @param [in, out] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int getInt(const char* key, int& value) = 0;

  /**
   * get unsigned int value of the json
   * @param [in] key
   *        the key name
   * @param [in, out] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int getUInt(const char* key, unsigned int& value) = 0;

  /**
   * get double value of the json
   * @param [in] key
   *        the key name
   * @param [in, out] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int getNumber(const char* key, double& value) = 0;

  /**
   * get string value of the json
   * @param [in] key
   *        the key name
   * @param [in, out] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int getString(const char* key, agora::util::AString& value) = 0;

  /**
   * get a child object value of the json
   * @param [in] key
   *        the key name
   * @param [in, out] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int getObject(const char* key, agora::util::AString& value) = 0;

  /**
   * get array value of the json
   * @param [in] key
   *        the key name
   * @param [in, out] value
   *        the value
   * @return return 0 if success or an error code
   */
  virtual int getArray(const char* key, const char* args,
                       agora::util::AString& value) = 0;

  /**
   * set parameters of the sdk or engine
   * @param [in] parameters
   *        the parameters
   * @return return 0 if success or an error code
   */
  virtual int setParameters(const char* parameters) = 0;

  virtual int convertPath(const char* filePath,
                          agora::util::AString& value) = 0;

  virtual ~IAgoraParameter(){}
};

class IEngineBase {
 public:
  virtual int queryInterface(INTERFACE_ID_TYPE iid, void** inter) = 0;
  virtual ~IEngineBase(){}
};

class AParameter : public agora::util::AutoPtr<IAgoraParameter> {
 public:
  AParameter(IEngineBase& engine) { initialize(&engine); }
  AParameter(IEngineBase* engine) { initialize(engine); }
  AParameter(IAgoraParameter* p) : agora::util::AutoPtr<IAgoraParameter>(p) {}

 private:
  bool initialize(IEngineBase* engine) {
    IAgoraParameter* p = NULL;
    if (engine &&
        !engine->queryInterface(AGORA_IID_PARAMETER_ENGINE, (void**)&p))
      reset(p);
    return p != NULL;
  }
};

}  // namespace base
}  // namespace agora

/**
 * to get the version number of the SDK
 * @param [in, out] build
 *        the build number of Agora SDK
 * @return returns the string of the version of the SDK
 */
AGORA_API const char* AGORA_CALL getAgoraSdkVersion(int* build);

/**
 * create the RTC engine object and return the pointer
 * @param [in] err
 *        the error code
 * @return returns the description of the error code
 */
AGORA_API const char* AGORA_CALL getAgoraSdkErrorDescription(int err);

/**
 * create the Agora Service object and return the pointer
 * @return returns pointer of the Agora Service object
 */
AGORA_API agora::base::IAgoraService* AGORA_CALL createAgoraService();

AGORA_API int AGORA_CALL
setAgoraSdkExternalSymbolLoader(void* (*func)(const char* symname));
