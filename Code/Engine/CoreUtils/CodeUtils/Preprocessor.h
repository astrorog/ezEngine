#include <CoreUtils/Basics.h>
#include <CoreUtils/CodeUtils/Tokenizer.h>
#include <Foundation/IO/Stream.h>
#include <Foundation/Logging/Log.h>
#include <Foundation/Containers/Map.h>

class EZ_COREUTILS_DLL ezPreprocessor
{
public:
  typedef ezResult (*FileOpenCB)(const char* szFile, ezDynamicArray<ezUInt8>& FileContent);
  typedef ezHybridArray<const ezToken*, 32> TokenStream;
  typedef ezDeque<TokenStream> MacroParameters;

public: // *** General ***
  ezPreprocessor();

  void SetLogInterface(ezLogInterface* pLog);

  ezResult ProcessFile(const char* szFile, ezStringBuilder& sOutput);

public: // *** File Handling ***
  void SetFileOpenCallback(FileOpenCB cb);

public: // *** Macro Definition ***

  void ClearDefines();
  bool RemoveDefine(const char* szName);
  ezResult AddDefine(const TokenStream& Tokens, ezUInt32& uiCurToken);

private: // *** General ***
  ezLogInterface* m_pLog;
  ezDeque<bool> m_IfdefActiveStack;

  ezResult ProcessCmd(const TokenStream& Tokens, ezStringBuilder& sOutput);

private: // *** File Handling ***
  ezResult OpenFile(const char* szFile, ezTokenizer** pTokenizer);

  FileOpenCB m_FileOpenCallback;
  ezMap<ezString, ezTokenizer> m_FileCache;

private: // *** Macro Definition ***

  struct MacroDefinition
  {
    MacroDefinition();

    const ezToken* m_MacroIdentifier;
    bool m_bIsFunction;
    bool m_bCurrentlyExpanding;
    bool m_bHasVarArgs;
    ezInt32 m_iNumParameters;
    TokenStream m_Replacement;
  };

  ezResult StoreDefine(const ezToken* pMacroNameToken, const TokenStream* pReplacementTokens, ezUInt32 uiFirstReplacementToken, ezInt32 iNumParameters, bool bUsesVarArgs);
  ezResult ExtractParameterName(const TokenStream& Tokens, ezUInt32& uiCurToken, ezString& sIdentifierName);

  ezMap<ezString, MacroDefinition> m_Macros;

  static const ezInt32 s_MacroParameter0 = ezTokenType::ENUM_COUNT + 2;
  static ezString s_ParamNames[32];
  ezToken m_ParameterTokens[32];

private: // *** #if condition parsing ***

  ezResult EvaluateCondition(const TokenStream& Tokens, ezUInt32& uiCurToken, ezInt32& iResult);
  ezResult ParseCondition(const TokenStream& Tokens, ezUInt32& uiCurToken, ezInt32& iResult);
  ezResult ParseFactor(const TokenStream& Tokens, ezUInt32& uiCurToken, ezInt32& iResult);
  ezResult ParseExpressionMul(const TokenStream& Tokens, ezUInt32& uiCurToken, ezInt32& iResult);
  ezResult ParseExpressionOr(const TokenStream& Tokens, ezUInt32& uiCurToken, ezInt32& iResult);
  ezResult ParseExpressionAnd(const TokenStream& Tokens, ezUInt32& uiCurToken, ezInt32& iResult);
  ezResult ParseExpressionPlus(const TokenStream& Tokens, ezUInt32& uiCurToken, ezInt32& iResult);
  ezResult ParseExpressionShift(const TokenStream& Tokens, ezUInt32& uiCurToken, ezInt32& iResult);


private: // *** Parsing ***
  static void SkipWhitespace(const TokenStream& Tokens, ezUInt32& uiCurToken);
  static void SkipWhitespaceAndNewline(const TokenStream& Tokens, ezUInt32& uiCurToken);
  static bool IsEndOfLine(const TokenStream& Tokens, ezUInt32 uiCurToken, bool bIgnoreWhitespace);
  static void CopyRelevantTokens(const TokenStream& Source, ezUInt32 uiFirstSourceToken, TokenStream& Destination);
  ezResult CopyTokensAndEvaluateDefined(const TokenStream& Source, ezUInt32 uiFirstSourceToken, TokenStream& Destination);

  bool Accept(const TokenStream& Tokens, ezUInt32& uiCurToken, const char* szToken, ezUInt32* pAccepted = nullptr);
  bool Accept(const TokenStream& Tokens, ezUInt32& uiCurToken, ezTokenType::Enum Type, ezUInt32* pAccepted = nullptr);
  bool Accept(const TokenStream& Tokens, ezUInt32& uiCurToken, const char* szToken1, const char* szToken2, ezUInt32* pAccepted = nullptr);
  bool AcceptUnless(const TokenStream& Tokens, ezUInt32& uiCurToken, const char* szToken1, const char* szToken2, ezUInt32* pAccepted = nullptr);

  ezResult Expect(const TokenStream& Tokens, ezUInt32& uiCurToken, const char* szToken, ezUInt32* pAccepted = nullptr);
  ezResult Expect(const TokenStream& Tokens, ezUInt32& uiCurToken, ezTokenType::Enum Type, ezUInt32* pAccepted = nullptr);
  ezResult Expect(const TokenStream& Tokens, ezUInt32& uiCurToken, const char* szToken1, const char* szToken2, ezUInt32* pAccepted = nullptr);
  ezResult ExpectEndOfLine(const TokenStream& Tokens, ezUInt32& uiCurToken);

  void CopyTokensReplaceParams(const TokenStream& Source, ezUInt32 uiFirstSourceToken, TokenStream& Destination, const ezHybridArray<ezString, 16>& parameters);
  ezResult ValidCodeCheck(const TokenStream& Tokens);
  void CombineTokensToString(const TokenStream& Tokens, ezUInt32 uiCurToken, ezStringBuilder& sResult);

private: // *** Macro Expansion ***
  ezResult Expand(const TokenStream& Tokens, TokenStream& Output);
  ezResult ExpandObjectMacro(MacroDefinition& Macro, TokenStream& Output);
  ezResult ExpandFunctionMacro(MacroDefinition& Macro, const MacroParameters& Parameters, TokenStream& Output);
  ezResult ExpandMacroParam(const ezToken& MacroToken, ezUInt32 uiParam, TokenStream& Output);
  void PassThroughFunctionMacro(MacroDefinition& Macro, const MacroParameters& Parameters, TokenStream& Output);
  ezToken* AddCustomToken(const ezToken* pPrevious, const char* szNewText);
  void OutputNotExpandableMacro(MacroDefinition& Macro, TokenStream& Output);
  ezResult ExtractAllMacroParameters(const TokenStream& Tokens, ezUInt32& uiCurToken, ezDeque< TokenStream >& AllParameters);
  ezResult ExtractParameterValue(const TokenStream& Tokens, ezUInt32& uiCurToken, TokenStream& ParamTokens);

  struct CustomToken
  {
    ezToken m_Token;
    ezString m_sIdentifierString;
  };

  enum TokenFlags : ezUInt32
  {
    NoFurtherExpansion = EZ_BIT(0),
  };

  const ezToken* m_TokenOpenParenthesis;
  const ezToken* m_TokenClosedParenthesis;
  const ezToken* m_TokenComma;
  
  ezDeque<const MacroParameters*> m_MacroParamStack;
  ezDeque<const MacroParameters*> m_MacroParamStackExpanded;
  ezDeque<CustomToken> m_CustomTokens;
};

#define PP_LOG(Type, FormatStr, ErrorToken, ...) \
  ezLog::Type(m_pLog, "Line %u (%u): "FormatStr, ErrorToken->m_uiLine, ErrorToken->m_uiColumn, __VA_ARGS__);
