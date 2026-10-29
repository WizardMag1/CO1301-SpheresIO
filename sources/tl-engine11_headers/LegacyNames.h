/**************************************************************************************************
	LegacyNames.h

	Old names used in previous TL-Engine versions to allow old code to compile
**************************************************************************************************/

#ifndef TL_LEGACY_NAMES_H_INCLUDED
#define TL_LEGACY_NAMES_H_INCLUDED

namespace tle {       // TL-Engine interface

class TLEngine;
using I3DEngine = TLEngine;

class Mesh;
using IMesh = Mesh;

class SceneNode;
using ISceneNode = SceneNode;

class Model;
using IModel = Model;

class Light;
using ILight = Light;

class Camera;
using ICamera = Camera;

class Sprite;
using ISprite = Sprite;

class Font;
using IFont = Font;


// Forced to use #define for enum types
#define EEngineType     EngineType
#define kTLX            TLXStandard
#define kTLXLegacy      TLXLegacy
// kIrrlicht is not supported any more

#define ECameraType     CameraType;
#define kManual         ManualCamera
#define kFPS            FPSCamera
					    
#define ELightType      LightType
#define kDirectional    DirectionalLight
#define kPoint          PointLight
#define kSpot           SpotLight

#define EHorizAlignment HorizontalAlignment
#define kLeft           LeftAlign
#define kCentre         CentreAlign
#define kRight          RightAlign

#define EVertAlignment  VerticalAlignment
#define kTop            TopAlign
#define kVCentre        MiddleAlign
#define kBottom         BottomAlign

#define EKeyState       KeyState
#define kNotPressed     KeyUp
#define kPressed        KeyStruck
#define kHeld           KeyDown

#define EKeyCode        KeyCode
#define kMaxKeyCodes    MaxKeyCodes


const unsigned int kWhite     =  White;
const unsigned int kBlack     =  Black;
const unsigned int kRed       =  Red;
const unsigned int kGreen     =  Green;
const unsigned int kBlue      =  Blue;
const unsigned int kCyan      =  Cyan;
const unsigned int kMagenta   =  Magenta;
const unsigned int kYellow    =  Yellow;
const unsigned int kLightGrey =  LightGrey;
const unsigned int kMidGrey   =  MidGrey;
const unsigned int kGrey      =  Grey;
const unsigned int kDarkGrey  =  DarkGrey;


} // namespace tle

#endif // TL_LEGACY_NAMES_H_INCLUDED