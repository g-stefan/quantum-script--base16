//
// Quantum Script Extension Base16
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16_VERSION_HPP
#define QUANTUM_SCRIPT_EXTENSION_BASE16_VERSION_HPP

#define QUANTUM_SCRIPT_EXTENSION_BASE16_VERSION_ABCD                 2,3,0,20
#define QUANTUM_SCRIPT_EXTENSION_BASE16_VERSION_STR                 "2.3.0"
#define QUANTUM_SCRIPT_EXTENSION_BASE16_VERSION_STR_BUILD           "20"
#define QUANTUM_SCRIPT_EXTENSION_BASE16_VERSION_STR_DATETIME        "2021-11-15 16:51:50"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16__EXPORT_HPP
#include "quantum-script-extension-base16--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Base16 {
				namespace Version {
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *version();
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *build();
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *versionWithBuild();
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *datetime();
				};
			};
		};
	};
};

#endif
#endif

