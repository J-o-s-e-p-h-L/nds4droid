/*
	Copyright (C) 2012 Jeffrey Quesnelle

	This file is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	This file is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with the this software.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cpu-features.h>
#include <jni.h>

#define JNI_NOARGS(X) Java_com_opendoorstudios_ds4droid_DeSmuME_##X(JNIEnv* env, jclass* clazz)

#define CPUTYPE_COMPAT 0
#define CPUTYPE_V7 1
#define CPUTYPE_V8 2
#define CPUTYPE_A9 3
#define CPUTYPE_A15 4
#define CPUTYPE_X86 5
#define CPUTYPE_X86_64 6

extern "C"
{

jint JNI_NOARGS(getCPUType)
{
	AndroidCpuFamily cpuFamily = android_getCpuFamily();
	uint64_t cpuFeatures = android_getCpuFeatures();
	if (cpuFamily == ANDROID_CPU_FAMILY_ARM64)
	{
		return CPUTYPE_V8;
	}
	else if (cpuFamily == ANDROID_CPU_FAMILY_ARM)
	{
		if ((cpuFeatures & ANDROID_CPU_ARM_FEATURE_IDIV_ARM) != 0)
		{
			return CPUTYPE_A15; // Anything past -a15 would benefit from this anyway
		}
		else if ((cpuFeatures & ANDROID_CPU_ARM_FEATURE_NEON) != 0)
		{
			return CPUTYPE_A9; // Anything not containing idiv but NEON should be cortex-a9
		}
		else if ((cpuFeatures & ANDROID_CPU_ARM_FEATURE_ARMv7) != 0)
		{
			return CPUTYPE_V7;
		}
	}
	else if (cpuFamily == ANDROID_CPU_FAMILY_X86_64)
	{
		return CPUTYPE_X86_64;
	}
	else if (cpuFamily == ANDROID_CPU_FAMILY_X86)
	{
		return CPUTYPE_X86;
	}
    else
    {
        return CPUTYPE_COMPAT;
    }
}

jint JNI_NOARGS(getCPUFamily)
{
	return (int)android_getCpuFamily();
}

}