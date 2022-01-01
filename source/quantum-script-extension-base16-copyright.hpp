//
// Quantum Script Extension Base16
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16_COPYRIGHT_HPP
#define QUANTUM_SCRIPT_EXTENSION_BASE16_COPYRIGHT_HPP

#define QUANTUM_SCRIPT_EXTENSION_BASE16_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define QUANTUM_SCRIPT_EXTENSION_BASE16_PUBLISHER            "Grigore Stefan"
#define QUANTUM_SCRIPT_EXTENSION_BASE16_COMPANY              QUANTUM_SCRIPT_EXTENSION_BASE16_PUBLISHER
#define QUANTUM_SCRIPT_EXTENSION_BASE16_CONTACT              "g_stefan@yahoo.com"
#define QUANTUM_SCRIPT_EXTENSION_BASE16_FULL_COPYRIGHT       QUANTUM_SCRIPT_EXTENSION_BASE16_COPYRIGHT " <" QUANTUM_SCRIPT_EXTENSION_BASE16_CONTACT ">"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16__EXPORT_HPP
#include "quantum-script-extension-base16--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Base16 {
				namespace Copyright {
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *copyright();
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *publisher();
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *company();
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *contact();
					QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT const char *fullCopyright();
				};
			};
		};
	};
};

#endif
#endif
