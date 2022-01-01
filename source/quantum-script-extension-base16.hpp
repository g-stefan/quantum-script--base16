//
// Quantum Script Extension Base16
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16_HPP
#define QUANTUM_SCRIPT_EXTENSION_BASE16_HPP

#ifndef QUANTUM_SCRIPT_HPP
#include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16__EXPORT_HPP
#include "quantum-script-extension-base16--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16_COPYRIGHT_HPP
#include "quantum-script-extension-base16-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16_LICENSE_HPP
#include "quantum-script-extension-base16-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_BASE16_VERSION_HPP
#include "quantum-script-extension-base16-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Base16 {

				using namespace Quantum::Script;

				QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_BASE16_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif

