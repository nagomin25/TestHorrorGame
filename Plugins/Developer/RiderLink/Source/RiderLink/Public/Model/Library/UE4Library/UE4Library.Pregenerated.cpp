//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a RdGen v1.13.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#include "UE4Library.Pregenerated.h"

#include "UE4Library/StringRange.Pregenerated.h"
#include "UE4Library/PlayState.Pregenerated.h"
#include "UE4Library/RequestSucceed.Pregenerated.h"
#include "UE4Library/RequestFailed.Pregenerated.h"
#include "UE4Library/LogMessageInfo.Pregenerated.h"
#include "UE4Library/UnrealLogEvent.Pregenerated.h"
#include "UE4Library/UClass.Pregenerated.h"
#include "UE4Library/BlueprintFunction.Pregenerated.h"
#include "UE4Library/ScriptCallStackFrame.Pregenerated.h"
#include "UE4Library/EmptyScriptCallStack.Pregenerated.h"
#include "UE4Library/ScriptCallStack.Pregenerated.h"
#include "UE4Library/UnableToDisplayScriptCallStack.Pregenerated.h"
#include "UE4Library/ScriptMsgException.Pregenerated.h"
#include "UE4Library/ScriptMsgCallStack.Pregenerated.h"
#include "UE4Library/BlueprintHighlighter.Pregenerated.h"
#include "UE4Library/BlueprintReference.Pregenerated.h"
#include "UE4Library/ConnectionInfo.Pregenerated.h"
#include "UE4Library/NotificationType.Pregenerated.h"
#include "UE4Library/RequestResultBase_Unknown.Pregenerated.h"
#include "UE4Library/IScriptCallStack_Unknown.Pregenerated.h"
#include "UE4Library/IScriptMsg_Unknown.Pregenerated.h"

#include "UE4Library/UE4Library.Pregenerated.h"
#include "UE4Library/UE4Library.Pregenerated.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable:4250 )
#pragma warning( disable:4307 )
#pragma warning( disable:4267 )
#pragma warning( disable:4244 )
#pragma warning( disable:4100 )
#endif

namespace JetBrains {
namespace EditorPlugin {
// companion

UE4Library::UE4LibrarySerializersOwner const UE4Library::serializersOwner;

void UE4Library::UE4LibrarySerializersOwner::registerSerializersCore(rd::Serializers const& serializers) const
{
    serializers.registry<StringRange>();
    serializers.registry<RequestSucceed>();
    serializers.registry<RequestFailed>();
    serializers.registry<LogMessageInfo>();
    serializers.registry<UnrealLogEvent>();
    serializers.registry<UClass>();
    serializers.registry<BlueprintFunction>();
    serializers.registry<ScriptCallStackFrame>();
    serializers.registry<EmptyScriptCallStack>();
    serializers.registry<ScriptCallStack>();
    serializers.registry<UnableToDisplayScriptCallStack>();
    serializers.registry<ScriptMsgException>();
    serializers.registry<ScriptMsgCallStack>();
    serializers.registry<BlueprintHighlighter>();
    serializers.registry<BlueprintReference>();
    serializers.registry<ConnectionInfo>();
    serializers.registry<RequestResultBase_Unknown>();
    serializers.registry<IScriptCallStack_Unknown>();
    serializers.registry<IScriptMsg_Unknown>();
}

void UE4Library::connect(rd::Lifetime lifetime, rd::IProtocol const * protocol)
{
    UE4Library::serializersOwner.registry(protocol->get_serializers());
    
    identify(*(protocol->get_identity()), rd::RdId::Null().mix("UE4Library"));
    bind(lifetime, protocol, "UE4Library");
}

// constants
// initializer
void UE4Library::initialize()
{
    serializationHash = 4930583670691204680L;
}
// primary ctor
// secondary constructor
// default ctors and dtors
UE4Library::UE4Library()
{
    initialize();
}
// reader
// writer
// virtual init
void UE4Library::init(rd::Lifetime lifetime) const
{
    rd::RdExtBase::init(lifetime);
}
// identify
void UE4Library::identify(const rd::Identities &identities, rd::RdId const &id) const
{
    rd::RdBindableBase::identify(identities, id);
}
// getters
// intern
// equals trait
// equality operators
bool operator==(const UE4Library &lhs, const UE4Library &rhs) {
    return &lhs == &rhs;
}
bool operator!=(const UE4Library &lhs, const UE4Library &rhs){
    return !(lhs == rhs);
}
// hash code trait
// type name trait
// static type name trait
// polymorphic to string
std::string UE4Library::toString() const
{
    std::string res = "UE4Library\n";
    return res;
}
// external to string
std::string to_string(const UE4Library & value)
{
    return value.toString();
}
}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

