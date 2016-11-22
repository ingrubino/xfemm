/* Copyright 2016 Johannes Zarl-Zierl <johannes.zarl-zierl@jku.at>
 *
 * The source code in this file is heavily derived from
 * FEMM by David Meeker <dmeeker@ieee.org>.
 * For more information on FEMM see http://www.femm.info
 * This modified version is not endorsed in any way by the original
 * authors of FEMM.
 *
 * License:
 * This software is subject to the Aladdin Free Public Licence
 * version 8, November 18, 1999.
 * The full license text is available in the file LICENSE.txt supplied
 * along with the source code.
 */

#include "LuaBaseCommands.h"

#include "FemmState.h"
#include "LuaInstance.h"
#include "fsolver.h"

#include <lua.h>

#include <iostream>
#include <memory>
#include <string>
#ifdef WIN32
#include <direct.h> // _chdir
#else
#include <unistd.h> // chdir
#endif

#ifdef DEBUG_FEMMLUA
#define debug std::cerr
#else
#define debug while(false) std::cerr
#endif

using namespace femm;

void femmcli::LuaBaseCommands::registerCommands(LuaInstance &li)
{
    li.addFunction("_ALERT",luaError);
    li.addFunction("messagebox",luaMessageBox);
    li.addFunction("pause",luaPause);
    //li.addFunction("prompt",luaPromptBox);
    li.addFunction("open",luaOpenDocument);
    li.addFunction("quit",luaExit);
    li.addFunction("exit",luaExit);
    li.addFunction("setcurrentdirectory",luaSetWorkingDirectory);
    li.addFunction("chdir",luaSetWorkingDirectory);

    li.addFunction("create",luaNewDocument);
    li.addFunction("newdocument",luaNewDocument);
    li.addFunction("new_document",luaNewDocument);

    li.addFunction("showconsole"  ,luaShowConsole);
    li.addFunction("show_console"  ,luaShowConsole);
    li.addFunction("showpointprops"  ,luaShowPointProps);
    li.addFunction("hidepointprops"  ,luaHidePointProps);
    li.addFunction("show_point_props",luaShowPointProps);
    li.addFunction("hide_point_props",luaHidePointProps);

    //lua_register(lua,"flput",lua_to_filelink);
    //lua_register(lua,"smartmesh",lua_smartmesh);
    //lua_register(lua,"makeplot",lua_makeplot);
}

/**
 * @brief Print an error message.
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_endapp()}
 */
int femmcli::LuaBaseCommands::luaError(lua_State *L)
{
    // Somthing went wrong in lua execution
    std::cerr << lua_tostring(L,1) << std::endl;

    return 0;
}

/**
 * @brief Dummy-function for compatibility.
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_endapp()}
 */
int femmcli::LuaBaseCommands::luaExit(lua_State *)
{
    debug << "NOP: luaExit" << std::endl;
    return 0;
}

/**
 * @brief Dummy-function for compatibility.
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_hidepointprops()}
 */
int femmcli::LuaBaseCommands::luaHidePointProps(lua_State *)
{
    debug << "NOP: luaHidePointProps" << std::endl;
    return 0;
}

/**
 * @brief Write a message.
 * Usually, this would go to a messagebox, but since this is a text implementation, print it to stdout.
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_messagebox()}
 */
int femmcli::LuaBaseCommands::luaMessageBox(lua_State *L)
{
    std::cout << "* " << lua_tostring(L,1) << std::endl;
    return 0;
}

/**
 * @brief Create new document
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_newdocument()}
 */
int femmcli::LuaBaseCommands::luaNewDocument(lua_State *L)
{
    int docType = static_cast<int>(lua_tonumber(L,1).Re());
    auto femmState = std::dynamic_pointer_cast<FemmState>(LuaInstance::instance(L)->femmState());

    // FIXME IMPLEMENT:
    // 1. get "CDocTemplate"
    // 2. call [MFC] CDocTemplate::OpenDocumentFile(NULL)
    switch (docType) {
        case 0: // magnetics
            //femmState->magneticsDocument = std::make_shared<MagneticsDocument>();
            break;
        case 1: // electrostatics
        case 2: // heat flow
        case 3: // current flow
            debug << "NOP: luaNewDocument("<<docType<<")" << std::endl;
            break;
        default:
            // we don't need to handle other docTypes that are used in femm
            // -> the other types are gui-specific
            debug << "document type " << docType << "not supported.\n";
            break;
    }
    return 0;
}

/**
 * @brief Open a document
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,luaOpenDocument()}
 */
int femmcli::LuaBaseCommands::luaOpenDocument(lua_State *L)
{
    std::string filename = lua_tostring(L,1);
    // FIXME IMPLEMENT
    debug << "NOP: luaOpenDocument("<<filename<<")" << std::endl;
    return 0;
}

/**
 * @brief Dummy-function for compatibility.
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_afxpause()}
 */
int femmcli::LuaBaseCommands::luaPause(lua_State *)
{
    debug << "NOP: luaPause" << std::endl;
    return 0;
}

/**
 * @brief Dummy-function for compatibility.
 * @return 0
 * \ingroup LuaCommon
 */
int femmcli::LuaBaseCommands::luaShowConsole(lua_State *)
{
    return 0;
}

/**
 * @brief Dummy-function for compatibility.
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_showpointprops()}
 */
int femmcli::LuaBaseCommands::luaShowPointProps(lua_State *)
{
    debug << "NOP: luaShowPointProps" << std::endl;
    return 0;
}

/**
 * @brief Set/Change working Directory.
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_setcurrentdirectory()}
 */
int femmcli::LuaBaseCommands::luaSetWorkingDirectory(lua_State *L)
{
    if (lua_gettop(L) != 0)
    {
        std::string newDirectory { lua_tostring(L,1) };
#ifdef WIN32
        _chdir(newDirectory.c_str());
#else
        chdir(newDirectory.c_str());
#endif
    }
    return 0;
}

/**
 * @brief Dummy-function for compatibility.
 * @param L
 * @return 0
 * \ingroup LuaCommon
 * \femm42{femm/femm.cpp,lua_makeplot()}
 */
int femmcli::LuaBaseCommands::luaMakePlot(lua_State *)
{
    debug << "NOP: luaMakePlot" << std::endl;
    return 0;
}


// vi:expandtab:tabstop=4 shiftwidth=4: