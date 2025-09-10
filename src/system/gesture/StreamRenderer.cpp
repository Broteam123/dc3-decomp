#include "gesture/StreamRenderer.h"
#include "math/Color.h"
#include "obj/Object.h"
#include "rndobj/Draw.h"

StreamRenderer::StreamRenderer()
    : mOutputTex(this), mForceMips(false), mDisplay(kStreamColor), mNumBlurs(4),
      mPlayer1DepthColor(1, 1, 1), mPlayer2DepthColor(0.48, 0.57, 0.8),
      mPlayer3DepthColor(0.05, 0.06, 0.54), mPlayer4DepthColor(0, 0, 0, 0),
      mPlayer5DepthColor(0, 0, 0, 0), mPlayer6DepthColor(0, 0, 0, 0),
      mPlayerDepthNobody(0, 0, 0, 0), mPlayer1DepthPalette(this),
      mPlayer2DepthPalette(this), mPlayerOtherDepthPalette(this),
      mBackgroundDepthPalette(this), mPlayer1DepthPaletteOffset(0),
      mPlayer2DepthPaletteOffset(0), mPlayerOtherDepthPaletteOffset(0),
      mBackgroundDepthPaletteOffset(0), mDrawPreClear(0), mForceDraw(0),
      mStaticColorIndices(0), mPCTestTex(this), mLagPrimaryTexture(0), unk154(0),
      unk190(0), unk194(0), unk198(0), unk19c(0), unk1a0(0), unk1a4(0), unk1a8(0),
      unk1ac(0) {
    for (int i = 0; i < 6; i++) {
        mSmoothers[i].SetSmoothParameters(6, 0);
    }
    mLaggedPrimaryTexture[0] = 0;
    mLaggedPrimaryTexture[1] = 0;
}

StreamRenderer::~StreamRenderer() {
    delete mLaggedPrimaryTexture[0];
    delete mLaggedPrimaryTexture[1];
}

BEGIN_PROPSYNCS(StreamRenderer)
    SYNC_PROP_MODIFY(output_texture, mOutputTex, SetOutputTex())
    SYNC_PROP_MODIFY(force_mips, mForceMips, SetOutputTex())
    SYNC_PROP(display, (int &)mDisplay)
    SYNC_PROP(num_blurs, mNumBlurs)
    SYNC_PROP(player_depth_nobody, mPlayerDepthNobody)
    SYNC_PROP(player_depth_nobody_alpha, mPlayerDepthNobody.alpha)
    SYNC_PROP(player1_depth_color, mPlayer1DepthColor)
    SYNC_PROP(player1_depth_color_alpha, mPlayer1DepthColor.alpha)
    SYNC_PROP(player2_depth_color, mPlayer2DepthColor)
    SYNC_PROP(player2_depth_color_alpha, mPlayer2DepthColor.alpha)
    SYNC_PROP(player3_depth_color, mPlayer3DepthColor)
    SYNC_PROP(player3_depth_color_alpha, mPlayer3DepthColor.alpha)
    SYNC_PROP(player4_depth_color, mPlayer4DepthColor)
    SYNC_PROP(player4_depth_color_alpha, mPlayer4DepthColor.alpha)
    SYNC_PROP(player5_depth_color, mPlayer5DepthColor)
    SYNC_PROP(player5_depth_color_alpha, mPlayer5DepthColor.alpha)
    SYNC_PROP(player6_depth_color, mPlayer6DepthColor)
    SYNC_PROP(player6_depth_color_alpha, mPlayer6DepthColor.alpha)
    SYNC_PROP(player1_depth_palette, mPlayer1DepthPalette)
    SYNC_PROP(player1_depth_palette_offset, mPlayer1DepthPaletteOffset)
    SYNC_PROP(player2_depth_palette, mPlayer2DepthPalette)
    SYNC_PROP(player2_depth_palette_offset, mPlayer2DepthPaletteOffset)
    SYNC_PROP(player_other_depth_palette, mPlayerOtherDepthPalette)
    SYNC_PROP(player_other_depth_palette_offset, mPlayerOtherDepthPaletteOffset)
    SYNC_PROP(background_depth_palette, mBackgroundDepthPalette)
    SYNC_PROP(background_depth_palette_offset, mBackgroundDepthPaletteOffset)
    SYNC_PROP(force_draw, mForceDraw)
    SYNC_PROP(static_color_indices, mStaticColorIndices)
    SYNC_PROP_MODIFY(draw_pre_clear, mDrawPreClear, UpdatePreClearState())
    SYNC_PROP(pc_test_texture, mPCTestTex)
    SYNC_PROP(lag_primary_texture, mLagPrimaryTexture)
    SYNC_PROP(crew_photo_edge_iterations, mCrewPhotoEdgeIterations)
    SYNC_PROP(crew_photo_edge_offset, mCrewPhotoEdgeOffset)
    SYNC_PROP(crew_photo_horizontal_color, mCrewPhotoHorizontalColor)
    SYNC_PROP(crew_photo_vertical_color, mCrewPhotoVerticalColor)
    SYNC_PROP(crew_photo_blur_start, mCrewPhotoBlurStart)
    SYNC_PROP(crew_photo_blur_width, mCrewPhotoBlurWidth)
    SYNC_PROP(crew_photo_blur_iterations, mCrewPhotoBlurIterations)
    SYNC_PROP(crew_photo_background_brightness, mCrewPhotoBackgroundBrightness)
    SYNC_SUPERCLASS(RndDrawable)
    SYNC_SUPERCLASS(Hmx::Object)
END_PROPSYNCS
