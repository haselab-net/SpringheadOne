// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "type.h"
#include "type_imp.h"

namespace LVRM {
#include "tokendef.h"
}

namespace LVRM {

bool TType::SetupType()
{
	const static TTypeProto::ProtoInit		pi[] = {
		{0,	0,	"Anchor"},
		{EVENTIN,	MFNODE,	"addChildren"},
		{EVENTIN,	MFNODE,	"removeChildren"},
		{EXPOSEDFIELD,	MFNODE,	"children"},	//	[]
		{EXPOSEDFIELD,	SFSTRING,	"description"},	//	""
		{EXPOSEDFIELD,	MFSTRING,	"parameter"},	//	[]
		{EXPOSEDFIELD,	MFSTRING,	"url"},	//	[]
		{FIELD,	SFVEC3F,	"bboxCenter"},	//	0 0 0     # (-,)
		{FIELD,	SFVEC3F,	"bboxSize"},	//	-1 -1 -1  # (0,) or -1,-1,-1

		{0,	0,	"Appearance"},
		{EXPOSEDFIELD,	SFNODE,	"material"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"texture"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"textureTransform"},	//	NULL

		{0,	0,	"AudioClip"},
		{EXPOSEDFIELD,	SFSTRING,	"description"},	//	""
		{EXPOSEDFIELD,	SFBOOL,	"loop"},	//	FALSE
		{EXPOSEDFIELD,	SFFLOAT,	"pitch"},	//	1.0        # (0,)
		{EXPOSEDFIELD,	SFTIME,	"startTime"},	//	0          # (-,)
		{EXPOSEDFIELD,	SFTIME,	"stopTime"},	//	0          # (-,)
		{EXPOSEDFIELD,	MFSTRING,	"url"},	//	[]
		{EVENTOUT,	SFTIME,	"duration_changed"},
		{EVENTOUT,	SFBOOL,	"isActive"},

		{0,	0,	"Background"},
		{EVENTIN,	SFBOOL,	"set_bind"},
		{EXPOSEDFIELD,	MFFLOAT,	"groundAngle"},	//	[]         # [0,PI/2]
		{EXPOSEDFIELD,	MFCOLOR,	"groundColor"},	//	[]         # [0,1]
		{EXPOSEDFIELD,	MFSTRING,	"backUrl"},	//	[]
		{EXPOSEDFIELD,	MFSTRING,	"bottomUrl"},	//	[]
		{EXPOSEDFIELD,	MFSTRING,	"frontUrl"},	//	[]
		{EXPOSEDFIELD,	MFSTRING,	"leftUrl"},	//	[]
		{EXPOSEDFIELD,	MFSTRING,	"rightUrl"},	//	[]
		{EXPOSEDFIELD,	MFSTRING,	"topUrl"},	//	[]
		{EXPOSEDFIELD,	MFFLOAT,	"skyAngle"},	//	[]         # [0,PI]
		{EXPOSEDFIELD,	MFCOLOR,	"skyColor"},	//	0 0 0      # [0,1]
		{EVENTOUT,	SFBOOL,	"isBound"},

		{0,	0,	"Billboard"},
		{EVENTIN,	MFNODE,	"addChildren"},
		{EVENTIN,	MFNODE,	"removeChildren"},
		{EXPOSEDFIELD,	SFVEC3F,	"axisOfRotation"},	//	0 1 0     # (-,)
		{EXPOSEDFIELD,	MFNODE,	"children"},	//	[]
		{FIELD,	SFVEC3F,	"bboxCenter"},	//	0 0 0     # (-,)
		{FIELD,	SFVEC3F,	"bboxSize"},	//	-1 -1 -1  # (0,) or -1,-1,-1

		{0,	0,	"Box"},
		{FIELD,	SFVEC3F,	"size"},	//	2 2 2        # (0,	)

		{0,	0,	"Collision"},
		{EVENTIN,	MFNODE,	"addChildren"},
		{EVENTIN,	MFNODE,	"removeChildren"},
		{EXPOSEDFIELD,	MFNODE,	"children"},	//	[]
		{EXPOSEDFIELD,	SFBOOL,	"collide"},	//	TRUE
		{FIELD,	SFVEC3F,	"bboxCenter"},	//	0 0 0      # (-,)
		{FIELD,	SFVEC3F,	"bboxSize"},	//	-1 -1 -1   # (0,) or -1,-1,-1
		{FIELD,	SFNODE,	"proxy"},	//	NULL
		{EVENTOUT,	SFTIME,	"collideTime"},

		{0,	0,	"Color"},
		{EXPOSEDFIELD,	MFCOLOR,	"color"},	//	[]         # [0,1]

		{0,	0,	"ColorInterpolator"},
		{EVENTIN,	SFFLOAT,	"set_fraction"},	//	# (-,)
		{EXPOSEDFIELD,	MFFLOAT,	"key"},	//	[]    # (-,)
		{EXPOSEDFIELD,	MFCOLOR,	"keyValue"},	//	[]    # [0,1]
		{EVENTOUT,	SFCOLOR,	"value_changed"},

		{0,	0,	"Cone"},
		{FIELD,	SFFLOAT,	"bottomRadius"},	//	1        # (0,)
		{FIELD,	SFFLOAT,	"height"},	//	2        # (0,)
		{FIELD,	SFBOOL,	"side"},	//	TRUE
		{FIELD,	SFBOOL,	"bottom"},	//	TRUE

		{0,	0,	"Coordinate"},
		{EXPOSEDFIELD,	MFVEC3F,	"point"},	//	[]      # (-,)

		{0,	0,	"CoordinateInterpolator"},
		{EVENTIN,	SFFLOAT,	"set_fraction"},	//	# (-,)
		{EXPOSEDFIELD,	MFFLOAT,	"key"},	//	[]    # (-,)
		{EXPOSEDFIELD,	MFVEC3F,	"keyValue"},	//	[]    # (-,)
		{EVENTOUT,	MFVEC3F,	"value_changed"},

		{0,	0,	"Cylinder"},
		{FIELD,	SFBOOL,	"bottom"},	//	TRUE
		{FIELD,	SFFLOAT,	"height"},	//	2         # (0,)
		{FIELD,	SFFLOAT,	"radius"},	//	1         # (0,)
		{FIELD,	SFBOOL,	"side"},	//	TRUE
		{FIELD,	SFBOOL,	"top"},	//	TRUE

		{0,	0,	"CylinderSensor"},
		{EXPOSEDFIELD,	SFBOOL,	"autoOffset"},	//	TRUE
		{EXPOSEDFIELD,	SFFLOAT,	"diskAngle"},	//	0.262       # (0,PI/2)
		{EXPOSEDFIELD,	SFBOOL,	"enabled"},	//	TRUE
		{EXPOSEDFIELD,	SFFLOAT,	"maxAngle"},	//	-1          # [-2PI,2PI]
		{EXPOSEDFIELD,	SFFLOAT,	"minAngle"},	//	0           # [-2PI,2PI]
		{EXPOSEDFIELD,	SFFLOAT,	"offset"},	//	0           # (-,)
		{EVENTOUT,	SFBOOL,	"isActive"},
		{EVENTOUT,	SFROTATION,	"rotation_changed"},
		{EVENTOUT,	SFVEC3F,	"trackPoint_changed"},

		{0,	0,	"DirectionalLight"},
		{EXPOSEDFIELD,	SFFLOAT,	"ambientIntensity"},	//	0        # [0,1]
		{EXPOSEDFIELD,	SFCOLOR,	"color"},	//	1 1 1    # [0,1]
		{EXPOSEDFIELD,	SFVEC3F,	"direction"},	//	0 0 -1   # (-,)
		{EXPOSEDFIELD,	SFFLOAT,	"intensity"},	//	1        # [0,1]
		{EXPOSEDFIELD,	SFBOOL,	"on"},	//	TRUE

		{0,	0,	"ElevationGrid"},
		{EVENTIN,	MFFLOAT,	"set_height"},
		{EXPOSEDFIELD,	SFNODE,	"color"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"normal"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"texCoord"},	//	NULL
		{FIELD,	MFFLOAT,	"height"},	//	[]      # (-,)
		{FIELD,	SFBOOL,	"ccw"},	//	TRUE
		{FIELD,	SFBOOL,	"colorPerVertex"},	//	TRUE
		{FIELD,	SFFLOAT,	"creaseAngle"},	//	0       # [0,PI]
		{FIELD,	SFBOOL,	"normalPerVertex"},	//	TRUE
		{FIELD,	SFBOOL,	"solid"},	//	TRUE
		{FIELD,	SFINT32,	"xDimension"},	//	0       # [0,)
		{FIELD,	SFFLOAT,	"xSpacing"},	//	1.0     # (0,)
		{FIELD,	SFINT32,	"zDimension"},	//	0       # [0,)
		{FIELD,	SFFLOAT,	"zSpacing"},	//	1.0     # (0,)

		{0,	0,	"Extrusion"},
		{EVENTIN,	MFVEC2F,	"set_crossSection"},
		{EVENTIN,	MFROTATION,	"set_orientation"},
		{EVENTIN,	MFVEC2F,	"set_scale"},
		{EVENTIN,	MFVEC3F,	"set_spine"},
		{FIELD,	SFBOOL,	"beginCap"},	//	TRUE
		{FIELD,	SFBOOL,	"ccw"},	//	TRUE
		{FIELD,	SFBOOL,	"convex"},	//	TRUE
		{FIELD,	SFFLOAT,	"creaseAngle"},	//	0                # [0,)
		{FIELD,	MFVEC2F,	"crossSection"},	//	[ 1 1, 1 -1, -1 -1, -1 1, 1  1 ]    # (-,)
		{FIELD,	SFBOOL,	"endCap"},	//	TRUE
		{FIELD,	MFROTATION,	"orientation"},	//	0 0 1 0          # [-1,1],(-,)
		{FIELD,	MFVEC2F,	"scale"},	//	1 1              # (0,)
		{FIELD,	SFBOOL,	"solid"},	//	TRUE
		{FIELD,	MFVEC3F,	"spine"},	//	[ 0 0 0,	0 1 0 ] # (-,)

		{0,	0,	"Fog"},
		{EXPOSEDFIELD,	SFCOLOR,	"color"},	//	1 1 1      # [0,1]
		{EXPOSEDFIELD,	SFSTRING,	"fogType"},	//	"LINEAR"
		{EXPOSEDFIELD,	SFFLOAT,	"visibilityRange"},	//	0          # [0,)
		{EVENTIN,	SFBOOL,	"set_bind"},
		{EVENTOUT,	SFBOOL,	"isBound"},

		{0,	0,	"FontStyle"},
		{FIELD,	MFSTRING,	"family"},	//	"SERIF"
		{FIELD,	SFBOOL,	"horizontal"},	//	TRUE
		{FIELD,	MFSTRING,	"justify"},	//	"BEGIN"
		{FIELD,	SFSTRING,	"language"},	//	""
		{FIELD,	SFBOOL,	"leftToRight"},	//	TRUE
		{FIELD,	SFFLOAT,	"size"},	//	1.0          # (0,)
		{FIELD,	SFFLOAT,	"spacing"},	//	1.0          # [0,)
		{FIELD,	SFSTRING,	"style"},	//	"PLAIN"
		{FIELD,	SFBOOL,	"topToBottom"},	//	TRUE

		{0,	0,	"Group"},
		{EVENTIN,	MFNODE,	"addChildren"},
		{EVENTIN,	MFNODE,	"removeChildren"},
		{EXPOSEDFIELD,	MFNODE,	"children"},	//	[]
		{FIELD,	SFVEC3F,	"bboxCenter"},	//	0 0 0     # (-,)
		{FIELD,	SFVEC3F,	"bboxSize"},	//	-1 -1 -1  # (0,) or -1,-1,-1

		{0,	0,	"ImageTexture"},
		{EXPOSEDFIELD,	MFSTRING,	"url"},	//	[]
		{FIELD,	SFBOOL,	"repeatS"},	//	TRUE
		{FIELD,	SFBOOL,	"repeatT"},	//	TRUE

		{0,	0,	"IndexedFaceSet"},
		{EVENTIN,	MFINT32,	"set_colorIndex"},
		{EVENTIN,	MFINT32,	"set_coordIndex"},
		{EVENTIN,	MFINT32,	"set_normalIndex"},
		{EVENTIN,	MFINT32,	"set_texCoordIndex"},
		{EXPOSEDFIELD,	SFNODE,	"color"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"coord"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"normal"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"texCoord"},	//	NULL
		{FIELD,	SFBOOL,	"ccw"},	//	TRUE
		{FIELD,	MFINT32,	"colorIndex"},	//	[]        # [-1,)
		{FIELD,	SFBOOL,	"colorPerVertex"},	//	TRUE
		{FIELD,	SFBOOL,	"convex"},	//	TRUE
		{FIELD,	MFINT32,	"coordIndex"},	//	[]        # [-1,)
		{FIELD,	SFFLOAT,	"creaseAngle"},	//	0         # [0,)
		{FIELD,	MFINT32,	"normalIndex"},	//	[]        # [-1,)
		{FIELD,	SFBOOL,	"normalPerVertex"},	//	TRUE
		{FIELD,	SFBOOL,	"solid"},	//	TRUE
		{FIELD,	MFINT32,	"texCoordIndex"},	//	[]        # [-1,)

		{0,	0,	"IndexedLineSet"},
		{EVENTIN,	MFINT32,	"set_colorIndex"},
		{EVENTIN,	MFINT32,	"set_coordIndex"},
		{EXPOSEDFIELD,	SFNODE,	"color"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"coord"},	//	NULL
		{FIELD,	MFINT32,	"colorIndex"},	//	[]     # [-1,)
		{FIELD,	SFBOOL,	"colorPerVertex"},	//	TRUE
		{FIELD,	MFINT32,	"coordIndex"},	//	[]     # [-1,)

		{0,	0,	"Inline"},
		{EXPOSEDFIELD,	MFSTRING,	"url"},	//	[]
		{EXPOSEDFIELD,	MFNODE,	"children"},	//	[]
		{FIELD,	SFVEC3F,	"bboxCenter"},	//	0 0 0     # (-,)
		{FIELD,	SFVEC3F,	"bboxSize"},	//	-1 -1 -1  # (0,) or -1,-1,-1

		{0,	0,	"LOD"},
		{EXPOSEDFIELD,	MFNODE,	"level"},	//	[]
		{FIELD,	SFVEC3F,	"center"},	//	0 0 0    # (-,)
		{FIELD,	MFFLOAT,	"range"},	//	[]       # (0,)

		{0,	0,	"Material"},
		{EXPOSEDFIELD,	SFFLOAT,	"ambientIntensity"},	//	0.2         # [0,1]
		{EXPOSEDFIELD,	SFCOLOR,	"diffuseColor"},	//	0.8 0.8 0.8 # [0,1]
		{EXPOSEDFIELD,	SFCOLOR,	"emissiveColor"},	//	0 0 0       # [0,1]
		{EXPOSEDFIELD,	SFFLOAT,	"shininess"},	//	0.2         # [0,1]
		{EXPOSEDFIELD,	SFCOLOR,	"specularColor"},	//	0 0 0       # [0,1]
		{EXPOSEDFIELD,	SFFLOAT,	"transparency"},	//	0           # [0,1]

		{0,	0,	"MovieTexture"},
		{EXPOSEDFIELD,	SFBOOL,	"loop"},	//	FALSE
		{EXPOSEDFIELD,	SFFLOAT,	"speed"},	//	1.0      # (-,)
		{EXPOSEDFIELD,	SFTIME,	"startTime"},	//	0        # (-,)
		{EXPOSEDFIELD,	SFTIME,	"stopTime"},	//	0        # (-,)
		{EXPOSEDFIELD,	MFSTRING,	"url"},	//	[]
		{FIELD,	SFBOOL,	"repeatS"},	//	TRUE
		{FIELD,	SFBOOL,	"repeatT"},	//	TRUE
		{EVENTOUT,	SFTIME,	"duration_changed"},
		{EVENTOUT,	SFBOOL,	"isActive"},

		{0,	0,	"NavigationInfo"},
		{EVENTIN,	SFBOOL,	"set_bind"},
		{EXPOSEDFIELD,	MFFLOAT,	"avatarSize"},	//	[0.25, 1.6, 0.75] # [0,)
		{EXPOSEDFIELD,	SFBOOL,	"headlight"},	//	TRUE
		{EXPOSEDFIELD,	SFFLOAT,	"speed"},	//	1.0               # [0,)
		{EXPOSEDFIELD,	MFSTRING,	"type"},	//	["WALK", "ANY"]
		{EXPOSEDFIELD,	SFFLOAT,	"visibilityLimit"},	//	0.0               # [0,)
		{EVENTOUT,	SFBOOL,	"isBound"},

		{0,	0,	"Normal"},
		{EXPOSEDFIELD,	MFVEC3F,	"vector"},	//	[]   # (-,)

		{0,	0,	"NormalInterpolator"},
		{EVENTIN,	SFFLOAT,	"set_fraction"},	//	# (-,)
		{EXPOSEDFIELD,	MFFLOAT,	"key"},	//	[]   # (-,)
		{EXPOSEDFIELD,	MFVEC3F,	"keyValue"},	//	[]   # (-,)
		{EVENTOUT,	MFVEC3F,	"value_changed"},

		{0,	0,	"OrientationInterpolator"},
		{EVENTIN,	SFFLOAT,	"set_fraction"},	//	# (-,)
		{EXPOSEDFIELD,	MFFLOAT,	"key"},	//	[]  # (-,)
		{EXPOSEDFIELD,	MFROTATION,	"keyValue"},	//	[]  # [-1,1],(-,)
		{EVENTOUT,	SFROTATION,	"value_changed"},

		{0,	0,	"PixelTexture"},
		{EXPOSEDFIELD,	SFIMAGE,	"image"},	//	0 0 0
		{FIELD,	SFBOOL,	"repeatS"},	//	TRUE
		{FIELD,	SFBOOL,	"repeatT"},	//	TRUE

		{0,	0,	"PlaneSensor"},
		{EXPOSEDFIELD,	SFBOOL,	"autoOffset"},	//	TRUE
		{EXPOSEDFIELD,	SFBOOL,	"enabled"},	//	TRUE
		{EXPOSEDFIELD,	SFVEC2F,	"maxPosition"},	//	-1 -1     # (-,)
		{EXPOSEDFIELD,	SFVEC2F,	"minPosition"},	//	0 0       # (-,)
		{EXPOSEDFIELD,	SFVEC3F,	"offset"},	//	0 0 0     # (-,)
		{EVENTOUT,	SFBOOL,	"isActive"},
		{EVENTOUT,	SFVEC3F,	"trackPoint_changed"},
		{EVENTOUT,	SFVEC3F,	"translation_changed"},

		{0,	0,	"PointLight"},
		{EXPOSEDFIELD,	SFFLOAT,	"ambientIntensity"},	//	0       # [0,1]
		{EXPOSEDFIELD,	SFVEC3F,	"attenuation"},	//	1 0 0   # [0,)
		{EXPOSEDFIELD,	SFCOLOR,	"color"},	//	1 1 1   # [0,1]
		{EXPOSEDFIELD,	SFFLOAT,	"intensity"},	//	1       # [0,1]
		{EXPOSEDFIELD,	SFVEC3F,	"location"},	//	0 0 0   # (-,)
		{EXPOSEDFIELD,	SFBOOL,	"on"},	//	TRUE
		{EXPOSEDFIELD,	SFFLOAT,	"radius"},	//	100     # [0,)

		{0,	0,	"PointSet"},
		{EXPOSEDFIELD,	SFNODE,	"color"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"coord"},	//	NULL

		{0,	0,	"PositionInterpolator"},
		{EVENTIN,	SFFLOAT,	"set_fraction"},	//	# (-,)
		{EXPOSEDFIELD,	MFFLOAT,	"key"},	//	[]    # (-,)
		{EXPOSEDFIELD,	MFVEC3F,	"keyValue"},	//	[]    # (-,)
		{EVENTOUT,	SFVEC3F,	"value_changed"},

		{0,	0,	"ProximitySensor"},
		{EXPOSEDFIELD,	SFVEC3F,	"center"},	//	0 0 0    # (-,)
		{EXPOSEDFIELD,	SFVEC3F,	"size"},	//	0 0 0    # [0,)
		{EXPOSEDFIELD,	SFBOOL,	"enabled"},	//	TRUE
		{EVENTOUT,	SFBOOL,	"isActive"},
		{EVENTOUT,	SFVEC3F,	"position_changed"},
		{EVENTOUT,	SFROTATION,	"orientation_changed"},
		{EVENTOUT,	SFTIME,	"enterTime"},
		{EVENTOUT,	SFTIME,	"exitTime"},

		{0,	0,	"ScalarInterpolator"},
		{EVENTIN,	SFFLOAT,	"set_fraction"},	//	# (-,)
		{EXPOSEDFIELD,	MFFLOAT,	"key"},	//	[]     # (-,)
		{EXPOSEDFIELD,	MFFLOAT,	"keyValue"},	//	[]     # (-,)
		{EVENTOUT,	SFFLOAT,	"value_changed"},

		{0,	0,	"Script"},
		{EXPOSEDFIELD,	MFSTRING,	"url"},	//	[]
		{FIELD,	SFBOOL,	"directOutput"},	//	FALSE
		{FIELD,	SFBOOL,	"mustEvaluate"},	//	FALSE

		{0,	0,	"Shape"},
		{EXPOSEDFIELD,	SFNODE,	"appearance"},	//	NULL
		{EXPOSEDFIELD,	SFNODE,	"geometry"},	//	NULL

		{0,	0,	"Sound"},
		{EXPOSEDFIELD,	SFVEC3F,	"direction"},	//	0 0 1   # (-,)
		{EXPOSEDFIELD,	SFFLOAT,	"intensity"},	//	1       # [0,1]
		{EXPOSEDFIELD,	SFVEC3F,	"location"},	//	0 0 0   # (-,)
		{EXPOSEDFIELD,	SFFLOAT,	"maxBack"},	//	10      # [0,)
		{EXPOSEDFIELD,	SFFLOAT,	"maxFront"},	//	10      # [0,)
		{EXPOSEDFIELD,	SFFLOAT,	"minBack"},	//	1       # [0,)
		{EXPOSEDFIELD,	SFFLOAT,	"minFront"},	//	1       # [0,)
		{EXPOSEDFIELD,	SFFLOAT,	"priority"},	//	0       # [0,1]
		{EXPOSEDFIELD,	SFNODE,	"source"},	//	NULL
		{FIELD,	SFBOOL,	"spatialize"},	//	TRUE

		{0,	0,	"Sphere"},
		{FIELD,	SFFLOAT,	"radius"},	//	1    # (0,)

		{0,	0,	"SphereSensor"},
		{EXPOSEDFIELD,	SFBOOL,	"autoOffset"},	//	TRUE
		{EXPOSEDFIELD,	SFBOOL,	"enabled"},	//	TRUE
		{EXPOSEDFIELD,	SFROTATION,	"offset"},	//	0 1 0 0  # [-1,1],(-,)
		{EVENTOUT,	SFBOOL,	"isActive"},
		{EVENTOUT,	SFROTATION,	"rotation_changed"},
		{EVENTOUT,	SFVEC3F,	"trackPoint_changed"},

		{0,	0,	"SpotLight"},
		{EXPOSEDFIELD,	SFFLOAT,	"ambientIntensity"},	//	0         # [0,1]
		{EXPOSEDFIELD,	SFVEC3F,	"attenuation"},	//	1 0 0     # [0,)
		{EXPOSEDFIELD,	SFFLOAT,	"beamWidth"},	//	1.570796  # (0,PI/2]
		{EXPOSEDFIELD,	SFCOLOR,	"color"},	//	1 1 1     # [0,1]
		{EXPOSEDFIELD,	SFFLOAT,	"cutOffAngle"},	//	0.785398  # (0,PI/2]
		{EXPOSEDFIELD,	SFVEC3F,	"direction"},	//	0 0 -1    # (-,)
		{EXPOSEDFIELD,	SFFLOAT,	"intensity"},	//	1         # [0,1]
		{EXPOSEDFIELD,	SFVEC3F,	"location"},	//	0 0 0     # (-,)
		{EXPOSEDFIELD,	SFBOOL,	"on"},	//	TRUE
		{EXPOSEDFIELD,	SFFLOAT,	"radius"},	//	100       # [0,)

		{0,	0,	"Switch"},
		{EXPOSEDFIELD,	MFNODE,	"choice"},	//	[]
		{EXPOSEDFIELD,	SFINT32,	"whichChoice"},	//	-1    # [-1,)

		{0,	0,	"Text"},
		{EXPOSEDFIELD,	MFSTRING,	"string"},	//	[]
		{EXPOSEDFIELD,	SFNODE,	"fontStyle"},	//	NULL
		{EXPOSEDFIELD,	MFFLOAT,	"length"},	//	[]      # [0,)
		{EXPOSEDFIELD,	SFFLOAT,	"maxExtent"},	//	0.0     # [0,)

		{0,	0,	"TextureCoordinate"},
		{EXPOSEDFIELD,	MFVEC2F,	"point"},	//	[]      # (-,)

		{0,	0,	"TextureTransform"},
		{EXPOSEDFIELD,	SFVEC2F,	"center"},	//	0 0     # (-,)
		{EXPOSEDFIELD,	SFFLOAT,	"rotation"},	//	0       # (-,)
		{EXPOSEDFIELD,	SFVEC2F,	"scale"},	//	1 1     # (-,)
		{EXPOSEDFIELD,	SFVEC2F,	"translation"},	//	0 0     # (-,)

		{0,	0,	"TimeSensor"},
		{EXPOSEDFIELD,	SFTIME,	"cycleInterval"},	//	1       # (0,)
		{EXPOSEDFIELD,	SFBOOL,	"enabled"},	//	TRUE
		{EXPOSEDFIELD,	SFBOOL,	"loop"},	//	FALSE
		{EXPOSEDFIELD,	SFTIME,	"startTime"},	//	0       # (-,)
		{EXPOSEDFIELD,	SFTIME,	"stopTime"},	//	0       # (-,)
		{EVENTOUT,	SFTIME,	"cycleTime"},
		{EVENTOUT,	SFFLOAT,	"fraction_changed"},	//	# [0,1]
		{EVENTOUT,	SFBOOL,	"isActive"},
		{EVENTOUT,	SFTIME,	"time"},

		{0,	0,	"TouchSensor"},
		{EXPOSEDFIELD,	SFBOOL,	"enabled"},	//	TRUE
		{EVENTOUT,	SFVEC3F,	"hitNormal_changed"},
		{EVENTOUT,	SFVEC3F,	"hitPoint_changed"},
		{EVENTOUT,	SFVEC2F,	"hitTexCoord_changed"},
		{EVENTOUT,	SFBOOL,	"isActive"},
		{EVENTOUT,	SFBOOL,	"isOver"},
		{EVENTOUT,	SFTIME,	"touchTime"},

		{0,	0,	"Transform"},
		{EVENTIN,	MFNODE,	"addChildren"},
		{EVENTIN,	MFNODE,	"removeChildren"},
		{EXPOSEDFIELD,	SFVEC3F,	"center"},	//	0 0 0    # (-,)
		{EXPOSEDFIELD,	MFNODE,	"children"},	//	[]
		{EXPOSEDFIELD,	SFROTATION,	"rotation"},	//	0 0 1 0  # [-1,1],(-,)
		{EXPOSEDFIELD,	SFVEC3F,	"scale"},	//	1 1 1    # (0,)
		{EXPOSEDFIELD,	SFROTATION,	"scaleOrientation"},	//	0 0 1 0  # [-1,1],(-,)
		{EXPOSEDFIELD,	SFVEC3F,	"translation"},	//	0 0 0    # (-,)
		{FIELD,	SFVEC3F,	"bboxCenter"},	//	0 0 0    # (-,)
		{FIELD,	SFVEC3F,	"bboxSize"},	//	-1 -1 -1 # (0,) or -1,-1,-1

		{0,	0,	"Viewpoint"},
		{EVENTIN,	SFBOOL,	"set_bind"},
		{EXPOSEDFIELD,	SFFLOAT,	"fieldOfView"},	//	0.785398  # (0,PI)
		{EXPOSEDFIELD,	SFBOOL,	"jump"},	//	TRUE
		{EXPOSEDFIELD,	SFROTATION,	"orientation"},	//	0 0 1 0   # [-1,1],(-,)
		{EXPOSEDFIELD,	SFVEC3F,	"position"},	//	0 0 10    # (-,)
		{FIELD,	SFSTRING,	"description"},	//	""
		{EVENTOUT,	SFTIME,	"bindTime"},
		{EVENTOUT,	SFBOOL,	"isBound"},

		{0,	0,	"VisibilitySensor"},
		{EXPOSEDFIELD,	SFVEC3F,	"center"},	//	0 0 0      # (-,)
		{EXPOSEDFIELD,	SFBOOL,	"enabled"},	//	TRUE
		{EXPOSEDFIELD,	SFVEC3F,	"size"},	//	0 0 0      # [0,)
		{EVENTOUT,	SFTIME,	"enterTime"},
		{EVENTOUT,	SFTIME,	"exitTime"},
		{EVENTOUT,	SFBOOL,	"isActive"},

		{0,	0,	"WorldInfo"},
		{FIELD,	MFSTRING,	"info"},	//	[]
		{FIELD,	SFSTRING,	"title"},	//	""

		{0,	0,	0},
	};
	const TTypeProto::ProtoInit	*p = pi;

	while(p < pi + sizeof(pi)/sizeof(*pi) && p->name){
		TTypeProto	*prt;
		if(!strcmp(p->name, "Transform")){
			prt = (TTypeProto*) new TTypeTransform(p->name);
		}else if(!strcmp(p->name, "Shape")){
			prt = (TTypeProto*) new TTypeShape(p->name);
		}else if(!strcmp(p->name, "Box")){
			prt = (TTypeProto*) new TTypeBox(p->name);
		}else if(!strcmp(p->name, "Sphere")){
			prt = (TTypeProto*) new TTypeSphere(p->name);
		}else if(!strcmp(p->name, "Cylinder")){
			prt = (TTypeProto*) new TTypeCylinder(p->name);
		}else if(!strcmp(p->name, "Cone")){
			prt = (TTypeProto*) new TTypeCone(p->name);
		}else if(!strcmp(p->name, "IndexedFaceSet")){
			prt = (TTypeProto*) new TTypeIndexedFaceSet(p->name);
		}else if(!strcmp(p->name, "ElevationGrid")){
			prt = (TTypeProto*) new TTypeElevationGrid(p->name);
		}else if(!strcmp(p->name, "TextureTransform")){
			prt = (TTypeProto*) new TTypeTextureTransform(p->name);
		}else if(!strcmp(p->name, "PixelTexture")){
			prt = (TTypeProto*) new TTypePixelTexture(p->name);
		}else if(!strcmp(p->name, "ImageTexture")){
			prt = (TTypeProto*) new TTypeImageTexture(p->name);
		}else if(!strcmp(p->name, "Material")){
			prt = (TTypeProto*) new TTypeMaterial(p->name);
		}else if(!strcmp(p->name, "Background")){
			prt = (TTypeProto*) new TTypeBackground(p->name);
		}else if(!strcmp(p->name, "Viewpoint")){
			prt = (TTypeProto*) new TTypeViewpoint(p->name);
		}else if(!strcmp(p->name, "Inline")){
			prt = (TTypeProto*) new TTypeInline(p->name);
		}else{
			prt = (TTypeProto*) new TTypeBase(p->name);
		}
		prt->SetFields(&++p);
	}
	return true;
}

}	//	namespace LVRM
