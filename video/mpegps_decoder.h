/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef VIDEO_MPEGPS_DECODER_H
#define VIDEO_MPEGPS_DECODER_H

#include "common/hashmap.h"
#include "graphics/surface.h"
#include "video/video_decoder.h"

namespace Audio {
class PacketizedAudioStream;
}

namespace Common {
class SeekableReadStream;
}

namespace Graphics {
struct PixelFormat;
}

namespace Image {
class MPEGDecoder;
}

namespace Video {

/**
 * Decoder for MPEG Program Stream videos.
 * Video decoder used in engines:
 *  - zvision
 */
class MPEGPSDecoder : public VideoDecoder {
public:
	MPEGPSDecoder();
	virtual ~MPEGPSDecoder();

	bool loadStream(Common::SeekableReadStream *stream);
	void close();

protected:
	void readNextPacket();
	bool useAudioSync() const { return false; }

private:
	// Base class for handling MPEG streams
	class MPEGStream {
	public:
		virtual ~MPEGStream() {}

		enum StreamType {
			kStreamTypeVideo,
			kStreamTypeAudio
		};

		virtual bool sendPacket(Common::SeekableReadStream *firstPacket, uint32 pts, uint32 dts) = 0;
		virtual StreamType getStreamType() const = 0;
	};

	// An MPEG 1/2 video track
	class MPEGVideoTrack : public VideoTrack, public MPEGStream {
	public:
		MPEGVideoTrack(Common::SeekableReadStream *firstPacket, const Graphics::PixelFormat &format);
		~MPEGVideoTrack();

		bool endOfTrack() const { return _endOfTrack; }
		uint16 getWidth() const;
		uint16 getHeight() const;
		Graphics::PixelFormat getPixelFormat() const;
		int getCurFrame() const { return _curFrame; }
		uint32 getNextFrameStartTime() const { return _nextFrameStartTime.msecs(); }
		const Graphics::Surface *decodeNextFrame();

		bool sendPacket(Common::SeekableReadStream *packet, uint32 pts, uint32 dts);
		StreamType getStreamType() const { return kStreamTypeVideo; }

		void setEndOfTrack() { _endOfTrack = true; }

	private:
		bool _endOfTrack;
		int _curFrame;
		Audio::Timestamp _nextFrameStartTime;
		Graphics::Surface *_surface;

		void findDimensions(Common::SeekableReadStream *firstPacket, const Graphics::PixelFormat &format);

#ifdef USE_MPEG2
		Image::MPEGDecoder *_mpegDecoder;
#endif
	};

#ifdef USE_MAD
	// An MPEG audio track
	class MPEGAudioTrack : public AudioTrack, public MPEGStream {
	public:
		MPEGAudioTrack(Common::SeekableReadStream &firstPacket, Audio::Mixer::SoundType soundType);
		~MPEGAudioTrack();

		bool sendPacket(Common::SeekableReadStream *packet, uint32 pts, uint32 dts);
		StreamType getStreamType() const { return kStreamTypeAudio; }

	protected:
		Audio::AudioStream *getAudioStream() const;

	private:
		Audio::PacketizedAudioStream *_audStream;
	};
#endif

	// The different types of private streams we can detect at the moment
	enum PrivateStreamType {
		kPrivateStreamUnknown,
		kPrivateStreamAC3,
		kPrivateStreamDTS,
		kPrivateStreamDVDPCM,
		kPrivateStreamPS2Audio
	};

	PrivateStreamType detectPrivateStreamType(Common::SeekableReadStream *packet);

	bool addFirstVideoTrack();

	int readNextPacketHeader(int32 &startCode, uint32 &pts, uint32 &dts);
	int findNextStartCode(uint32 &size);
	uint32 readPTS(int c);

	void parseProgramStreamMap(int length);
	byte _psmESType[256];

	// A map from stream types to stream handlers
	typedef Common::HashMap<int, MPEGStream *> StreamMap;
	StreamMap _streamMap;

	Common::SeekableReadStream *_stream;
};

} // End of namespace Video

#endif
