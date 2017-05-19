/*
// Copyright (c) 2017 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#include "IDiagnostic.h"
#include <utils/String8.h>
#include <binder/Parcel.h>
#include <binder/IInterface.h>

namespace intel {
namespace ufo {
namespace hwc {
namespace services {

using namespace android;

class BpDiagnostic : public BpInterface<IDiagnostic>
{
public:
    BpDiagnostic(const sp<IBinder>& impl)
        : BpInterface<IDiagnostic>(impl),
          mReply(0)
    {
    }

    enum {
        TRANSACT_READ_LOG_PARCEL = IBinder::FIRST_CALL_TRANSACTION,
        TRANSACT_ENABLE_DISPLAY,
        TRANSACT_DISABLE_DISPLAY,
        TRANSACT_MASK_LAYER,
        TRANSACT_DUMP_FRAMES
    };

    virtual ~BpDiagnostic()
    {
        if (mReply)
            delete mReply;
    }

    status_t readLogParcel(Parcel* reply)
    {
        Parcel data;
        data.writeInterfaceToken(IDiagnostic::getInterfaceDescriptor());
        status_t ret = remote()->transact(TRANSACT_READ_LOG_PARCEL, data, reply);
        if (ret != NO_ERROR)
        {
            ALOGW("%s() transact failed: %d", __FUNCTION__, ret);
        }
        return ret;
    }

    void enableDisplay(uint32_t d)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IDiagnostic::getInterfaceDescriptor());
        data.writeInt32(d);
        status_t ret = remote()->transact(TRANSACT_ENABLE_DISPLAY, data, &reply);
        if (ret != NO_ERROR) {
            ALOGW("%s() transact failed: %d", __FUNCTION__, ret);
            return;
        }
    }

    void disableDisplay(uint32_t d, bool bBlank)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IDiagnostic::getInterfaceDescriptor());
        data.writeInt32(d);
        data.writeInt32(bBlank);
        status_t ret = remote()->transact(TRANSACT_DISABLE_DISPLAY, data, &reply);
        if (ret != NO_ERROR) {
            ALOGW("%s() transact failed: %d", __FUNCTION__, ret);
            return;
        }
    }

    void maskLayer(uint32_t d, uint32_t layer, bool bHide)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IDiagnostic::getInterfaceDescriptor());
        data.writeInt32(d);
        data.writeInt32(layer);
        data.writeInt32(bHide);
        status_t ret = remote()->transact(TRANSACT_MASK_LAYER, data, &reply);
        if (ret != NO_ERROR) {
            ALOGW("%s() transact failed: %d", __FUNCTION__, ret);
            return;
        }
    }

    void dumpFrames(uint32_t d, int32_t frames, bool bSync)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IDiagnostic::getInterfaceDescriptor());
        data.writeInt32(d);
        data.writeInt32(frames);
        data.writeInt32(bSync);
        status_t ret = remote()->transact(TRANSACT_DUMP_FRAMES, data, &reply);
        if (ret != NO_ERROR) {
            ALOGW("%s() transact failed: %d", __FUNCTION__, ret);
            return;
        }
    }

private:
    Parcel* mReply;
};


IMPLEMENT_META_INTERFACE(Diagnostic, "intel.ufo.hwc.diagnostic");

status_t BnDiagnostic::onTransact(
        uint32_t code,
        const Parcel& data,
        Parcel* reply,
        uint32_t flags)
{
    switch(code) {
        case BpDiagnostic::TRANSACT_READ_LOG_PARCEL:
        {
            CHECK_INTERFACE(IDiagnostic, data, reply);
            status_t err = readLogParcel(reply);
            return err;
        }

        case BpDiagnostic::TRANSACT_ENABLE_DISPLAY:
        {
            CHECK_INTERFACE(IDiagnostic, data, reply);
            uint32_t d = data.readInt32();
            enableDisplay(d);
            return NO_ERROR;
        }

        case BpDiagnostic::TRANSACT_DISABLE_DISPLAY:
        {
            CHECK_INTERFACE(IDiagnostic, data, reply);
            uint32_t d = data.readInt32();
            bool bBlank = data.readInt32();
            disableDisplay(d, bBlank);
            return NO_ERROR;
        }

        case BpDiagnostic::TRANSACT_MASK_LAYER:
        {
            CHECK_INTERFACE(IDiagnostic, data, reply);
            uint32_t d = data.readInt32();
            uint32_t layer = data.readInt32();
            bool bHide = data.readInt32();
            maskLayer(d, layer, bHide);
            return NO_ERROR;
        }

        case BpDiagnostic::TRANSACT_DUMP_FRAMES:
        {
            CHECK_INTERFACE(IDiagnostic, data, reply);
            uint32_t d = data.readInt32();
            int32_t frames = data.readInt32();
            bool bSync = data.readInt32();
            dumpFrames(d, frames, bSync);
            return NO_ERROR;
        }

        default:
        return BBinder::onTransact(code, data, reply, flags);
    }
}

}   // services
}   // hwc
}   // ufo
}   // intel
