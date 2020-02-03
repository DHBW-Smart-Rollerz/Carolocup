/*
 * Stanley_Querregler.cpp
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Stanley_Querregler".
 *
 * Model version              : 1.11
 * Simulink Coder version : 9.2 (R2019b) 18-Jul-2019
 * C++ source code generated on : Wed Jan 29 19:20:14 2020
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Generic->Unspecified (assume 32-bit Generic)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Stanley_Querregler.h"
#include "Stanley_Querregler_private.h"

/* Block signals (default storage) */
B_Stanley_Querregler_T Stanley_Querregler_B;

/* Block states (default storage) */
DW_Stanley_Querregler_T Stanley_Querregler_DW;

/* Real-time model */
RT_MODEL_Stanley_Querregler_T Stanley_Querregler_M_ =
  RT_MODEL_Stanley_Querregler_T();
RT_MODEL_Stanley_Querregler_T *const Stanley_Querregler_M =
  &Stanley_Querregler_M_;

/* Forward declaration for local functions */
static void Stanle_angleUtilities_wrapTo2Pi(real_T *theta);
static void matlabCodegenHandle_matlabCod_a(ros_slros_internal_block_Subs_T *obj);
static void matlabCodegenHandle_matlabCodeg(ros_slros_internal_block_Publ_T *obj);

/*
 * System initialize for enable system:
 *    '<S3>/Enabled Subsystem'
 *    '<S4>/Enabled Subsystem'
 */
void Stanley_Q_EnabledSubsystem_Init(B_EnabledSubsystem_Stanley_Qu_T *localB,
  P_EnabledSubsystem_Stanley_Qu_T *localP)
{
  /* SystemInitialize for Outport: '<S6>/Out1' */
  localB->In1 = localP->Out1_Y0;
}

/*
 * Output and update for enable system:
 *    '<S3>/Enabled Subsystem'
 *    '<S4>/Enabled Subsystem'
 */
void Stanley_Querre_EnabledSubsystem(boolean_T rtu_Enable, const
  SL_Bus_Stanley_Querregler_std_msgs_Float32 *rtu_In1,
  B_EnabledSubsystem_Stanley_Qu_T *localB)
{
  /* Outputs for Enabled SubSystem: '<S3>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S6>/Enable'
   */
  if (rtu_Enable) {
    /* Inport: '<S6>/In1' */
    localB->In1 = *rtu_In1;
  }

  /* End of Outputs for SubSystem: '<S3>/Enabled Subsystem' */
}

/* Function for MATLAB Function: '<S9>/Kinematic' */
static void Stanle_angleUtilities_wrapTo2Pi(real_T *theta)
{
  boolean_T positiveInput;
  real_T x;
  boolean_T rEQ0;
  real_T q;
  positiveInput = (*theta > 0.0);
  x = *theta;
  if (rtIsNaN(*theta) || rtIsInf(*theta)) {
    *theta = (rtNaN);
  } else if (*theta == 0.0) {
    *theta = 0.0;
  } else {
    *theta = fmod(*theta, 6.2831853071795862);
    rEQ0 = (*theta == 0.0);
    if (!rEQ0) {
      q = fabs(x / 6.2831853071795862);
      rEQ0 = (fabs(q - floor(q + 0.5)) <= 2.2204460492503131E-16 * q);
    }

    if (rEQ0) {
      *theta = 0.0;
    } else {
      if (x < 0.0) {
        *theta += 6.2831853071795862;
      }
    }
  }

  positiveInput = ((*theta == 0.0) && positiveInput);
  *theta += 6.2831853071795862 * static_cast<real_T>(positiveInput);
}

static void matlabCodegenHandle_matlabCod_a(ros_slros_internal_block_Subs_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

static void matlabCodegenHandle_matlabCodeg(ros_slros_internal_block_Publ_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
  }
}

/* Model step function */
void Stanley_Querregler_step(void)
{
  SL_Bus_Stanley_Querregler_std_msgs_Float32 b_varargout_2;
  boolean_T b_varargout_1;
  SL_Bus_Stanley_Querregler_std_msgs_Float32 rtb_BusAssignment;
  real_T rtb_ms;
  real_T rtb_Product1;
  real_T d_idx_0;
  real_T d_idx_1;

  /* Outputs for Atomic SubSystem: '<Root>/Subscribe2' */
  /* MATLABSystem: '<S3>/SourceBlock' */
  b_varargout_1 = Sub_Stanley_Querregler_66.getLatestMessage(&b_varargout_2);

  /* Outputs for Enabled SubSystem: '<S3>/Enabled Subsystem' */
  Stanley_Querre_EnabledSubsystem(b_varargout_1, &b_varargout_2,
    &Stanley_Querregler_B.EnabledSubsystem);

  /* End of Outputs for SubSystem: '<S3>/Enabled Subsystem' */
  /* End of Outputs for SubSystem: '<Root>/Subscribe2' */

  /* Outputs for Atomic SubSystem: '<Root>/Subscribe3' */
  /* MATLABSystem: '<S4>/SourceBlock' */
  b_varargout_1 = Sub_Stanley_Querregler_67.getLatestMessage(&b_varargout_2);

  /* Outputs for Enabled SubSystem: '<S4>/Enabled Subsystem' */
  Stanley_Querre_EnabledSubsystem(b_varargout_1, &b_varargout_2,
    &Stanley_Querregler_B.EnabledSubsystem_e);

  /* End of Outputs for SubSystem: '<S4>/Enabled Subsystem' */
  /* End of Outputs for SubSystem: '<Root>/Subscribe3' */

  /* Product: '<S8>/Product1' incorporates:
   *  Constant: '<S8>/Constant'
   *  Constant: '<S8>/Radius (cm)'
   *  Gain: '<S8>/cm in m'
   *  Product: '<S8>/Product'
   */
  rtb_Product1 = Stanley_Querregler_P.cminm_Gain *
    Stanley_Querregler_P.Radiuscm_Value *
    Stanley_Querregler_B.EnabledSubsystem_e.In1.Data *
    Stanley_Querregler_P.Constant_Value_mh;

  /* Switch: '<S5>/Switch' incorporates:
   *  Constant: '<S5>/R�ckw�rts'
   *  Constant: '<S5>/Vorw�rts'
   */
  if (rtb_Product1 > Stanley_Querregler_P.Switch_Threshold) {
    rtb_ms = Stanley_Querregler_P.Vorwrts_Value;
  } else {
    rtb_ms = Stanley_Querregler_P.Rckwrts_Value;
  }

  /* End of Switch: '<S5>/Switch' */

  /* MATLAB Function: '<S9>/Kinematic' incorporates:
   *  Constant: '<S5>/Constant'
   *  Constant: '<S5>/Constant1'
   *  Constant: '<S5>/[x, y, theta]'
   *  Product: '<S5>/Matrix Multiply'
   *  Sum: '<S5>/Add'
   */
  Stanley_Querregler_B.refPose[0] = Stanley_Querregler_P.Constant1_Value[0] *
    Stanley_Querregler_B.EnabledSubsystem.In1.Data +
    Stanley_Querregler_P.xytheta_Value[0];
  Stanley_Querregler_B.currPose[0] = Stanley_Querregler_P.Constant_Value_h[0];
  Stanley_Querregler_B.refPose[1] = Stanley_Querregler_P.Constant1_Value[1] *
    Stanley_Querregler_B.EnabledSubsystem.In1.Data +
    Stanley_Querregler_P.xytheta_Value[1];
  Stanley_Querregler_B.currPose[1] = Stanley_Querregler_P.Constant_Value_h[1];
  Stanley_Querregler_B.refPose[2] = (Stanley_Querregler_P.Constant1_Value[2] *
    Stanley_Querregler_B.EnabledSubsystem.In1.Data +
    Stanley_Querregler_P.xytheta_Value[2]) * 0.017453292519943295;
  Stanle_angleUtilities_wrapTo2Pi(&Stanley_Querregler_B.refPose[2]);
  Stanley_Querregler_B.currPose[2] = 0.017453292519943295 *
    Stanley_Querregler_P.Constant_Value_h[2];
  Stanle_angleUtilities_wrapTo2Pi(&Stanley_Querregler_B.currPose[2]);
  if (rtb_ms == 1.0) {
    d_idx_0 = (Stanley_Querregler_P.Kinematic_Wheelbase * cos
               (Stanley_Querregler_B.currPose[2]) +
               Stanley_Querregler_P.Constant_Value_h[0]) -
      Stanley_Querregler_B.refPose[0];
    d_idx_1 = (Stanley_Querregler_P.Kinematic_Wheelbase * sin
               (Stanley_Querregler_B.currPose[2]) +
               Stanley_Querregler_P.Constant_Value_h[1]) -
      Stanley_Querregler_B.refPose[1];
  } else {
    d_idx_0 = Stanley_Querregler_B.currPose[0] - Stanley_Querregler_B.refPose[0];
    d_idx_1 = Stanley_Querregler_B.currPose[1] - Stanley_Querregler_B.refPose[1];
  }

  Stanley_Querregler_B.b = (Stanley_Querregler_B.currPose[2] -
    Stanley_Querregler_B.refPose[2]) + 3.1415926535897931;
  Stanle_angleUtilities_wrapTo2Pi(&Stanley_Querregler_B.b);
  if (rtb_ms == 1.0) {
    /* Switch: '<S5>/Switch1' incorporates:
     *  Gain: '<S5>/Gain'
     */
    if (!(rtb_Product1 > Stanley_Querregler_P.Switch1_Threshold)) {
      rtb_Product1 *= Stanley_Querregler_P.Gain_Gain;
    }

    rtb_Product1 = -(atan(-(d_idx_0 * sin(Stanley_Querregler_B.refPose[2]) -
      d_idx_1 * cos(Stanley_Querregler_B.refPose[2])) *
                          Stanley_Querregler_P.LateralControllerStanley_Positi /
                          (rtb_Product1 + 1.0)) + (Stanley_Querregler_B.b -
      3.1415926535897931));
  } else {
    if (rtb_ms == 1.0) {
      rtb_ms = Stanley_Querregler_P.LateralControllerStanley_Positi;
    } else {
      rtb_ms = Stanley_Querregler_P.LateralControllerStanley_Posi_o;
    }

    /* Switch: '<S5>/Switch1' incorporates:
     *  Gain: '<S5>/Gain'
     */
    if (!(rtb_Product1 > Stanley_Querregler_P.Switch1_Threshold)) {
      rtb_Product1 *= Stanley_Querregler_P.Gain_Gain;
    }

    rtb_Product1 = atan(-(d_idx_0 * sin(Stanley_Querregler_B.refPose[2]) -
                          d_idx_1 * cos(Stanley_Querregler_B.refPose[2])) *
                        rtb_ms / (rtb_Product1 + -1.0)) +
      (Stanley_Querregler_B.b - 3.1415926535897931);
  }

  rtb_Product1 *= 57.295779513082323;
  rtb_ms = fabs(rtb_Product1);
  if (rtb_Product1 < 0.0) {
    rtb_Product1 = -1.0;
  } else if (rtb_Product1 > 0.0) {
    rtb_Product1 = 1.0;
  } else if (rtb_Product1 == 0.0) {
    rtb_Product1 = 0.0;
  } else {
    rtb_Product1 = (rtNaN);
  }

  if ((!(rtb_ms < Stanley_Querregler_P.Kinematic_MaxSteeringAngle)) && (!rtIsNaN
       (Stanley_Querregler_P.Kinematic_MaxSteeringAngle))) {
    rtb_ms = Stanley_Querregler_P.Kinematic_MaxSteeringAngle;
  }

  /* BusAssignment: '<Root>/Bus Assignment' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   *  MATLAB Function: '<S9>/Kinematic'
   */
  rtb_BusAssignment.Data = static_cast<real32_T>((rtb_Product1 * rtb_ms));

  /* Outputs for Atomic SubSystem: '<Root>/Publish1' */
  /* MATLABSystem: '<S2>/SinkBlock' */
  Pub_Stanley_Querregler_65.publish(&rtb_BusAssignment);

  /* End of Outputs for SubSystem: '<Root>/Publish1' */
}

/* Model initialize function */
void Stanley_Querregler_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* block I/O */
  (void) memset((static_cast<void *>(&Stanley_Querregler_B)), 0,
                sizeof(B_Stanley_Querregler_T));

  /* states (dwork) */
  (void) memset(static_cast<void *>(&Stanley_Querregler_DW), 0,
                sizeof(DW_Stanley_Querregler_T));

  {
    char_T tmp[17];
    char_T tmp_0[13];
    char_T tmp_1[19];
    int32_T i;
    static const char_T tmp_2[18] = { '/', 's', 'p', 'u', 'r', 'm', 'i', 't',
      't', 'e', 'n', 'a', 'b', 's', 't', 'a', 'n', 'd' };

    static const char_T tmp_3[12] = { '/', 'r', 'a', 'd', 'd', 'r', 'e', 'h',
      'z', 'a', 'h', 'l' };

    static const char_T tmp_4[16] = { '/', 's', 'e', 'r', 'v', 'o', 'l', 'e',
      'n', 'k', 'w', 'i', 'n', 'k', 'e', 'l' };

    /* Start for Atomic SubSystem: '<Root>/Subscribe2' */
    /* Start for MATLABSystem: '<S3>/SourceBlock' */
    Stanley_Querregler_DW.obj_o.matlabCodegenIsDeleted = false;
    Stanley_Querregler_DW.objisempty_h = true;
    Stanley_Querregler_DW.obj_o.isInitialized = 1;
    for (i = 0; i < 18; i++) {
      tmp_1[i] = tmp_2[i];
    }

    tmp_1[18] = '\x00';
    Sub_Stanley_Querregler_66.createSubscriber(tmp_1, 1);
    Stanley_Querregler_DW.obj_o.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S3>/SourceBlock' */
    /* End of Start for SubSystem: '<Root>/Subscribe2' */

    /* Start for Atomic SubSystem: '<Root>/Subscribe3' */
    /* Start for MATLABSystem: '<S4>/SourceBlock' */
    Stanley_Querregler_DW.obj_p.matlabCodegenIsDeleted = false;
    Stanley_Querregler_DW.objisempty = true;
    Stanley_Querregler_DW.obj_p.isInitialized = 1;
    for (i = 0; i < 12; i++) {
      tmp_0[i] = tmp_3[i];
    }

    tmp_0[12] = '\x00';
    Sub_Stanley_Querregler_67.createSubscriber(tmp_0, 1);
    Stanley_Querregler_DW.obj_p.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S4>/SourceBlock' */
    /* End of Start for SubSystem: '<Root>/Subscribe3' */

    /* Start for Atomic SubSystem: '<Root>/Publish1' */
    /* Start for MATLABSystem: '<S2>/SinkBlock' */
    Stanley_Querregler_DW.obj.matlabCodegenIsDeleted = false;
    Stanley_Querregler_DW.objisempty_d = true;
    Stanley_Querregler_DW.obj.isInitialized = 1;
    for (i = 0; i < 16; i++) {
      tmp[i] = tmp_4[i];
    }

    tmp[16] = '\x00';
    Pub_Stanley_Querregler_65.createPublisher(tmp, 1);
    Stanley_Querregler_DW.obj.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S2>/SinkBlock' */
    /* End of Start for SubSystem: '<Root>/Publish1' */
  }

  /* SystemInitialize for Atomic SubSystem: '<Root>/Subscribe2' */
  /* SystemInitialize for Enabled SubSystem: '<S3>/Enabled Subsystem' */
  Stanley_Q_EnabledSubsystem_Init(&Stanley_Querregler_B.EnabledSubsystem,
    &Stanley_Querregler_P.EnabledSubsystem);

  /* End of SystemInitialize for SubSystem: '<S3>/Enabled Subsystem' */
  /* End of SystemInitialize for SubSystem: '<Root>/Subscribe2' */

  /* SystemInitialize for Atomic SubSystem: '<Root>/Subscribe3' */
  /* SystemInitialize for Enabled SubSystem: '<S4>/Enabled Subsystem' */
  Stanley_Q_EnabledSubsystem_Init(&Stanley_Querregler_B.EnabledSubsystem_e,
    &Stanley_Querregler_P.EnabledSubsystem_e);

  /* End of SystemInitialize for SubSystem: '<S4>/Enabled Subsystem' */
  /* End of SystemInitialize for SubSystem: '<Root>/Subscribe3' */
}

/* Model terminate function */
void Stanley_Querregler_terminate(void)
{
  /* Terminate for Atomic SubSystem: '<Root>/Subscribe2' */
  /* Terminate for MATLABSystem: '<S3>/SourceBlock' */
  matlabCodegenHandle_matlabCod_a(&Stanley_Querregler_DW.obj_o);

  /* End of Terminate for SubSystem: '<Root>/Subscribe2' */

  /* Terminate for Atomic SubSystem: '<Root>/Subscribe3' */
  /* Terminate for MATLABSystem: '<S4>/SourceBlock' */
  matlabCodegenHandle_matlabCod_a(&Stanley_Querregler_DW.obj_p);

  /* End of Terminate for SubSystem: '<Root>/Subscribe3' */

  /* Terminate for Atomic SubSystem: '<Root>/Publish1' */
  /* Terminate for MATLABSystem: '<S2>/SinkBlock' */
  matlabCodegenHandle_matlabCodeg(&Stanley_Querregler_DW.obj);

  /* End of Terminate for SubSystem: '<Root>/Publish1' */
}