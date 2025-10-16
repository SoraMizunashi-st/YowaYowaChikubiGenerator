// -------------------------------------------------------------------------------------------------------------------------------------//
//   _______  _______  _______  ___      ___      _______    ___      _______  _______                                                  //
//  |       ||       ||       ||   |    |   |    |   _   |  |   |    |   _   ||  _    |      YYCK For C++                               //
//  |  _____||_     _||    ___||   |    |   |    |  |_|  |  |   |    |  |_|  || |_|   |      Version  : 1.0.0                           //
//  | |_____   |   |  |   |___ |   |    |   |    |       |  |   |    |       ||       |      Author   : Sora Mizunashi                  //
//  |_____  |  |   |  |    ___||   |___ |   |___ |       |  |   |___ |       ||  _    | ___                                             //
//   _____| |  |   |  |   |___ |       ||       ||   _   |  |       ||   _   || |_|   ||   | SPDX-FileCopyrightText : 2025~ StellaLab.  //
//  |_______|  |___|  |_______||_______||_______||__| |__|  |_______||__| |__||_______||___| SPDX-License-Identifier: MIT               //
// -------------------------------------------------------------------------------------------------------------------------------------//
//  I am from a country where English is not my native language
//  The following explanation may contain inaccuracies. Please be aware of this.
// -------------------------------------------------------------------------------------------------------------------------------------//
// ◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆ Individual  Discription ◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆ //
// -------------------------------------------------------------------------------------------------------------------------------------//
//
// -------------------------------------------------------------------------------------------------------------------------------------//
// ◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆ Common  Description ◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆ //
// -------------------------------------------------------------------------------------------------------------------------------------//
// YYCG is a generator for creating action chains with the primary purpose of nipple teasing.
// Please refer to the reference for basic operation.
// URL: https://github.com/SoraMizunashi-st/YowaYowaChikubiGenerator
// For individual inquiries, please contact Mizunashi Sora. (StellaLab.)
// Email : shuiwukong6@gmail.com
// -------------------------------------------------------------------------------------------------------------------------------------//
// SENaM Project Coding Style and Naming Conventions
// This document outlines the standardized coding style and naming conventions for the SENaM C++ project.
// Consistent adherence to these rules ensures high readability, consistency, and maintainability across the codebase.
// -------------------------------------------------------------------------------------------------------------------------------------//
// I. Naming Conventions
// ・Classes, Structs, Enums, and Type Definitions: Use PascalCase (all words start with an uppercase letter).
// ・Member Variables: Prefix with a lowercase m (for member) and use PascalCase for the rest of the name.
// ・Static Variables: Prefix with a lowercase s (for static) and use PascalCase.
// ・Constant Variables: Prefix with a lowercase c (for Constant) and use PascalCase.
// ・Function/Method Arguments: Prefix with a lowercase p and use PascalCase for the rest of the name.
// ・Temporary/Local Variables: Prefix with a lowercase t (for temporary) and use PascalCase.
// ・Public Methods: Use PascalCase with no special prefix.
// -------------------------------------------------------------------------------------------------------------------------------------//
// II. Code Structure and Style
// Namespace Usage: All standard library entities must be referenced using their fully qualified names.
// Rule: Always use the std:: prefix.
// Class Definition Order: 
// The visibility sections within a class or struct must be defined in the following order to prioritize the user-facing interface:
// [1]public
// [2]private
// [3]protected (if applicable, typically placed last)
// General Principle: Prefer enum class for defining sets of named constants over macro definitions (#define) to ensure strong type safety.
// -------------------------------------------------------------------------------------------------------------------------------------//
// ◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆ License summary ◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆ //
// -------------------------------------------------------------------------------------------------------------------------------------//
// SPDX-FileCopyrightText : 2025~ StellaLab.
// SPDX-License-Identifier: MIT
// -------------------------------------------------------------------------------------------------------------------------------------//
// Please read below for specific information about spdx licenses.
// https://spdx.dev/learn/handling-license-info/
// -------------------------------------------------------------------------------------------------------------------------------------//
// Include Guarde
// -------------------------------------------------------------------------------------------------------------------------------------//
#ifndef YYCG_CORE_HPP
#define YYCG_CORE_HPP
// -------------------------------------------------------------------------------------------------------------------------------------//
#ifdef __YYCG_BUILD__
// -------------------------------------------------------------------------------------------------------------------------------------//

// -------------------------------------------------------------------------------------------------------------------------------------//
#else
// -------------------------------------------------------------------------------------------------------------------------------------//
#define __DEBUG__
// -------------------------------------------------------------------------------------------------------------------------------------//
#endif
// -------------------------------------------------------------------------------------------------------------------------------------//

// -------------------------------------------------------------------------------------------------------------------------------------//
// Types Object Header
// -------------------------------------------------------------------------------------------------------------------------------------//

// -------------------------------------------------------------------------------------------------------------------------------------//
// Utility Object  Header
// -------------------------------------------------------------------------------------------------------------------------------------//

// -------------------------------------------------------------------------------------------------------------------------------------//


// -------------------------------------------------------------------------------------------------------------------------------------//
// Custom Object Header
// -------------------------------------------------------------------------------------------------------------------------------------//
#include "./ListTokenIDs.hpp"
#include "./ListDecorateTokens.hpp"
#include "./TypeReservedConstants.hpp"
// -------------------------------------------------------------------------------------------------------------------------------------//


// -------------------------------------------------------------------------------------------------------------------------------------//
// External Library Header
// -------------------------------------------------------------------------------------------------------------------------------------//

// -------------------------------------------------------------------------------------------------------------------------------------//

// -------------------------------------------------------------------------------------------------------------------------------------//
// Standard Library Header
// -------------------------------------------------------------------------------------------------------------------------------------//
#include <iostream> // use <std::cout>
#include <string>   // use <string>

#include <memory>   // use < std::unique_ptr , std::make_uniquer >

#include <vector>
#include <algorithm>
#include <random> 

#include <chrono>
// -------------------------------------------------------------------------------------------------------------------------------------//

using TokenIDs  = std::vector<int>;
using TokenText = std::vector<std::string>;

using ScenarioTensor = std::vector<int>;

class yycgCore
{
public:
    yycgCore( std::string p_FileName );
    ~yycgCore() = default;

    // core->Setting().Generate().Result();
    yycgCore& Setting();
    yycgCore& Generate();
    yycgCore& Result();

private:

    std::string m_FileName;

    // Tokens
    TokenIDs  m_KeywordIDs;
    TokenText m_KeywordText;

    TokenIDs  m_TokenIDs;
    TokenText m_TokenText;

    // Tensor
    ScenarioTensor m_ScenarioTensor;
    ScenarioTensor m_AdditionalTransitionTensor;
    constexpr static int EmbeddingDimention = 16;

    // Number of generated tokens
    constexpr static int KeywordRanges = 64;

    // Restriction parameters, not yet used
    constexpr static int MaxRepeat = 500;
    constexpr static int MaxTokens = 50;

    // It's code logic so don't mess with it
    constexpr static int MinKeyword =  0;
    constexpr static int MaxKeyword = (int)YYCG::E_KEYWORD::LIST_SIZE;
    constexpr static int SwapCount = 15;

    // Tokens
    TokenIDs  helperRandomTokens();
    TokenText helperEncodeKeyword( TokenIDs p_Keyword );
    TokenIDs  helperSortTokens( TokenIDs p_Keyword );
    TokenIDs  helperRandomSwapTokens( TokenIDs p_Keyword );

    // Tensor
    ScenarioTensor helperEmbeddingTokenToScenarioTensor( TokenIDs p_Keyword );
    ScenarioTensor helperAdditionalScenarioTensor( ScenarioTensor p_tensor );

    // I'm not going to call it from outside anyway, 
    // so I'm wondering if I should change it to a direct call format.
    yycgCore& helperLogKeyword( [[maybe_unused]] TokenIDs  p_Keyword );
    yycgCore& helperLogText(    [[maybe_unused]] TokenText p_Keyword );
    yycgCore& helperScenarioLog([[maybe_unused]] ScenarioTensor p_Scenario );

};



// -------------------------------------------------------------------------------------------------------------------------------------//
// Include Guarde
// -------------------------------------------------------------------------------------------------------------------------------------//
#endif
// -------------------------------------------------------------------------------------------------------------------------------------//