// K3D proof-of-concept: a simple 8-bpc After Effects effect.
// It copies the input and inverts the RGB channels, retaining alpha.
#include "AEConfig.h"
#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include <cstdio>

extern "C" DllExport PF_Err EffectMain(PF_Cmd cmd, PF_InData* in_data,
    PF_OutData* out_data, PF_ParamDef* params[], PF_LayerDef* output, void*) {
    switch (cmd) {
    case PF_Cmd_ABOUT:
        std::snprintf(out_data->return_msg, sizeof(out_data->return_msg),
            "K3D Test v0.1\rBasic 8-bit test effect. Not a 3D renderer.");
        break;
    case PF_Cmd_GLOBAL_SETUP:
        out_data->my_version = PF_VERSION(1, 0, 0, PF_Stage_DEVELOP, 1);
        out_data->out_flags = 0;
        out_data->out_flags2 = 0;
        break;
    case PF_Cmd_PARAMS_SETUP:
        out_data->num_params = 1; // only the implicit input layer
        break;
    case PF_Cmd_RENDER: {
        PF_Err err = PF_COPY(&params[0]->u.ld, output, nullptr, nullptr);
        if (err) return err;
        for (A_long y = 0; y < output->height; ++y) {
            auto* row = reinterpret_cast<PF_Pixel8*>(
                reinterpret_cast<char*>(output->data) + y * output->rowbytes);
            for (A_long x = 0; x < output->width; ++x) {
                const auto a = row[x].alpha;
                row[x].red = row[x].red > a ? 0 : a - row[x].red;
                row[x].green = row[x].green > a ? 0 : a - row[x].green;
                row[x].blue = row[x].blue > a ? 0 : a - row[x].blue;
            }
        }
        break;
    }
    default: break;
    }
    return PF_Err_NONE;
}
