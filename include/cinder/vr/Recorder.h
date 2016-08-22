/*
 Copyright 2016 Google Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.


 Copyright (c) 2016, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/vr/Controller.h"
#include "cinder/Filesystem.h"

namespace cinder { namespace vr {

class Recorder;
using RecorderRef = std::shared_ptr<Recorder>;

//! \class Recorder
//!
//! All pose data is stored in the devices tracking coordinate system.
//!
class Recorder {
public:

	enum Mode { RECORD, PLAYBACK };
	
	//! \class ControllerInput
	//!
	//!
	struct Input {
		enum class Type : uint32_t { UNKNOWN = 0xFFFFFFFF, POSE = 0, BUTTON, TRIGGER, AXIS };
		Input::Type				mType = Input::Type::UNKNOWN;
		uint32_t				mSize = 0;
		std::vector<uint8_t>	mData;
	};

	//! \class Frame
	//!
	//!
	class Frame {
	public:
		Frame() {}
		Frame( uint32_t frame, double time ) : mFrame( frame ) {}
		uint32_t								getFrame() const { return mFrame; }
		double									getTime() const { return mTime; }
		const ci::mat4&							getHmdPose() const { return mHmdPose; }
		void									setHmdPose( const ci::mat4& pose ) { mHmdPose = pose; }
		void									appendInput( const Recorder::Input& input ) { mInput.push_back( input ); }
		const std::vector<Recorder::Input>&		getInput() const { return mInput; }
	private:
		uint32_t								mFrame = 0;
		double									mTime = 0;
		ci::mat4								mHmdPose;
		std::vector<Recorder::Input>			mInput;
	};

	// ---------------------------------------------------------------------------------------------

	static RecorderRef				create( Recorder::Mode mode );

	//! Advance to the next frame for recording
	virtual Recorder::Frame*		nextFrame( uint32_t frameNum, double elapsedSeconds );
	//! Advance to the next frame for playback
	virtual const Recorder::Frame*	nextFrame() const;
	
	virtual bool					read( const ci::fs::path& filePath );
	virtual bool					write( const ci::fs::path& filePath );

private:
	Recorder( Recorder::Mode mode );

	Recorder::Mode					mMode;
	std::vector<Recorder::Frame>	mFrames;
	mutable size_t					mCurrentFrameIndex = std::numeric_limits<size_t>::max();
};

}} // namespace cinder::vr