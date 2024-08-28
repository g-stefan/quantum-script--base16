// Quantum Script Extension Base16
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/Base16/Library.hpp>
#include <XYO/QuantumScript.Extension/Base16/Copyright.hpp>
#include <XYO/QuantumScript.Extension/Base16/License.hpp>
#include <XYO/QuantumScript.Extension/Base16/Version.hpp>
#include <XYO/QuantumScript.Extension/Buffer/VariableBuffer.hpp>

namespace XYO::QuantumScript::Extension::Base16 {

	static TPointer<Variable> encode(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- base16-encode\n");
#endif
		return VariableString::newVariable(XYO::Encoding::Base16::encode((arguments->index(0))->toString()));
	};

	static TPointer<Variable> decode(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- base16-decode\n");
#endif
		String result;
		if (XYO::Encoding::Base16::decode((arguments->index(0))->toString(), result)) {
			return VariableString::newVariable(result);
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> decodeToBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- base16-decode-to-buffer\n");
#endif
		String result;
		if (XYO::Encoding::Base16::decode((arguments->index(0))->toString(), result)) {
			return Extension::Buffer::VariableBuffer::newVariableFromString(result);
		};
		return Context::getValueUndefined();
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("Base16", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {
		String info = "Base16\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "Base16");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::Base16::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		executive->compileStringX("Script.requireExtension(\"Buffer\");");
		executive->compileStringX("var Base16={};");

		executive->setFunction2("Base16.encode(str)", encode);
		executive->setFunction2("Base16.decode(str)", decode);
		executive->setFunction2("Base16.decodeToBuffer(str)", decodeToBuffer);
	};

};

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_BASE16_LIBRARY
#	ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_BASE16_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::Base16::initExecutive(executive, extensionId);
};
#	endif
#endif
