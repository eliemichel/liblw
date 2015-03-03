
#include "lw/event/Promise.hpp"

namespace lw {
namespace event {

Promise::Promise( void ):
    m_state( new _SharedState() )
{
    m_state->resolved   = false;
    m_state->rejected   = false;
    m_state->handler    = nullptr;
}

// -------------------------------------------------------------------------- //

Promise::Promise( Promise&& other ):
    m_state( std::move( other.m_state ) )
{
    other.m_state = nullptr;
}

// -------------------------------------------------------------------------- //

Future Promise::future( void ){
    return Future( m_state );
}

// -------------------------------------------------------------------------- //

void Promise::resolve( void ){
    m_state->resolved = true;
    if( m_state->handler ){
        m_state->handler();
    }
}

// -------------------------------------------------------------------------- //

void Promise::reject( void ){
    m_state->rejected = true;
    if( m_state->handler ){
        m_state->handler();
    }
}

// -------------------------------------------------------------------------- //

Promise& Promise::operator=( Promise&& other ){
    m_state = std::move( other.m_state );
    other.m_state = nullptr;
    return *this;
}

// -------------------------------------------------------------------------- //

void Future::then( Promise&& promise ){
    auto next = std::make_shared< Promise >( std::move( promise ) );
    auto prev = m_state;
    m_state->handler = [ prev, next ]() mutable {
        if( next ){
            next->resolve();
        }
        prev.reset();
    };
}

}
}