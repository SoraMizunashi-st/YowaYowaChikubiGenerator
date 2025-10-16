#include "yycgCore.hpp"



// -------------------------------------------------------------------------------------------------------------------------------------//
// Constructor
// -------------------------------------------------------------------------------------------------------------------------------------//
yycgCore::yycgCore( std::string p_FileName )
    : m_FileName( std::move( p_FileName ))
    , m_KeywordIDs(KeywordRanges , 0)
    , m_TokenIDs( KeywordRanges , 0 )
    , m_ScenarioTensor( (KeywordRanges * EmbeddingDimention) , 0 )
    , m_AdditionalTransitionTensor( ((KeywordRanges * 2 ) - 1 ) * EmbeddingDimention , 0 )
{

}

yycgCore& yycgCore::Setting()
{
#ifdef __DEBUG__
    std::cout << "Start yycg | Core Logic " << std::endl;
#endif
    // Token Generate
    this->m_KeywordIDs  = this->helperRandomTokens();
    this->helperLogKeyword( this->m_KeywordIDs );

#ifdef __DEBUG__
    // Token To Text
    this->m_KeywordText = std::move(this->helperEncodeKeyword( this->m_KeywordIDs ));
    this->helperLogText( this->m_KeywordText );
#endif

    // Sort tokens by level
#ifdef __DEBUG__
    std::cout << "Sorting Tokens | Level : weak < --- > strong " << std::endl;
#endif

    this->m_KeywordIDs  = this->helperSortTokens( this->m_KeywordIDs );
    this->helperLogKeyword( this->m_KeywordIDs );

#ifdef __DEBUG__
    // Token To Text
    this->m_KeywordText = std::move(this->helperEncodeKeyword( this->m_KeywordIDs ));
    this->helperLogText( this->m_KeywordText );
#endif

    // Randomly swap talks to create ambiguity
#ifdef __DEBUG__
    std::cout << "Token Swapping | Randomly swap talks to create ambiguity " << std::endl;
#endif

    this->m_KeywordIDs  = this->helperRandomSwapTokens( this->m_KeywordIDs );
    this->helperLogKeyword( this->m_KeywordIDs );

#ifdef __DEBUG__
    // Token To Text
    this->m_KeywordText = std::move(this->helperEncodeKeyword( this->m_KeywordIDs ));
    this->helperLogText( this->m_KeywordText );
#endif

    return *this;
}

yycgCore& yycgCore::Generate()
{
    // Token To Scenario [ Token N * D( time , Decolate , Repeat ) ]
    this->m_ScenarioTensor = this->helperEmbeddingTokenToScenarioTensor( this->m_KeywordIDs );

#ifdef __DEBUG__
    this->helperScenarioLog( this->m_ScenarioTensor );
#endif

    // vector transition
    
    m_AdditionalTransitionTensor = this->helperAdditionalScenarioTensor( this->m_ScenarioTensor );

#ifdef __DEBUG__
    this->helperScenarioLog( this->m_AdditionalTransitionTensor );
#endif

    // QKV EQing


    // FFN


    // Softmax


    // Logit to Script

    return *this;
}

yycgCore& yycgCore::Result()
{
    // Decode | Script To TextFile

    return *this;
}


// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | Random keyword generation
// -------------------------------------------------------------------------------------------------------------------------------------//
TokenIDs yycgCore::helperRandomTokens()
{   
    std::mt19937 t_RandomGenerator( static_cast<unsigned int>(std::time(nullptr)) );
    std::uniform_int_distribution<> t_Distribution( MinKeyword , MaxKeyword -1 );

    TokenIDs t_Keyword;
    while ( t_Keyword.size() < KeywordRanges )
    {
        int t_RandomID = t_Distribution( t_RandomGenerator );
        t_Keyword.push_back( t_RandomID );
    }
    return t_Keyword; 

}

// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | TokenID( index : int ) to Keyword( Text : std::string )
// -------------------------------------------------------------------------------------------------------------------------------------//
TokenText yycgCore::helperEncodeKeyword( TokenIDs p_Keyword  )
{
    TokenText t_KeywordText; 
    
    if (p_Keyword.empty())
    {
        return t_KeywordText;
    }
    
    t_KeywordText.reserve(p_Keyword.size());

    for( size_t i = 0; i < p_Keyword.size(); ++i )
    {
        const int t_ID = p_Keyword[i];

        if( t_ID >= 0 && t_ID < (int)YYCG::E_KEYWORD::LIST_SIZE )
        {

            if ( (int)YYCG::KEYWORD_TABLE[t_ID].id == t_ID )
            {
                t_KeywordText.push_back( YYCG::KEYWORD_TABLE[t_ID].name );
            }
        }
    }

    return t_KeywordText;
}

// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | TokenID( index : int ) Sort
// -------------------------------------------------------------------------------------------------------------------------------------//
TokenIDs yycgCore::helperSortTokens( TokenIDs p_Keyword)
{
    // TokenIDs は std::vector<int> のエイリアスだと確定し、ラムダ引数を int で受け取ります。
    std::sort(p_Keyword.begin(), p_Keyword.end(), 
        // 💡 修正: ラムダの引数を const int& に変更
        [](const int& a_val, const int& b_val) {
            
            // int の値を E_KEYWORD にキャストします。
            const YYCG::E_KEYWORD a_id = static_cast<YYCG::E_KEYWORD>(a_val);
            const YYCG::E_KEYWORD b_id = static_cast<YYCG::E_KEYWORD>(b_val);
            
            // KEYWORD_TABLE からレベルを取得するためのインデックスに再度キャスト
            const size_t a_index = static_cast<size_t>(a_id);
            const size_t b_index = static_cast<size_t>(b_id);
            
            // 安全性チェック: 範囲外アクセスを防ぐ
            // YYCG::KEYWORD_TABLE がこのスコープで見える必要があります
            if (a_index >= YYCG::KEYWORD_TABLE.size() || b_index >= YYCG::KEYWORD_TABLE.size()) {
                // 不正なIDはソートの比較対象として影響を最小限にするため false を返す
                return false; 
            }

            // レベルを取得
            const int level_a = YYCG::KEYWORD_TABLE[a_index].level;
            const int level_b = YYCG::KEYWORD_TABLE[b_index].level;

            // レベルが低い方 (よわよわ) が前に来るようにソート (昇順)
            return level_a < level_b;
        }
    );

    return p_Keyword;
}

// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | TokenID( index : int ) Sort
// -------------------------------------------------------------------------------------------------------------------------------------//
TokenIDs yycgCore::helperRandomSwapTokens( TokenIDs p_Keyword)
{
    // 1. リストのサイズを取得
    const size_t list_size = p_Keyword.size();
    
    // リストの要素が2つ未満の場合、スワップは不要
    if (list_size < 2) {
        return p_Keyword;
    }
    
    // 2. 乱数生成器の準備
    // 現在時刻をシードとする（実際のプロジェクトではmt19937をクラスのメンバとして持つ方が効率的です）
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    
    // 3. 乱数分布 (0からリストサイズ-1までのインデックスを生成)
    std::uniform_int_distribution<size_t> distribution(0, list_size - 1);

    // 4. 指定回数だけスワップを実行
    for (int i = 0; i < SwapCount; ++i) {
        // ランダムに2つの異なるインデックスを選ぶ
        size_t index_a = distribution(generator);
        size_t index_b = distribution(generator);
        
        // 異なる要素を確実にスワップするため、同じインデックスを選んだ場合は再抽選する
        // ただし、リストサイズが大きい場合は稀なので、ここではシンプルに処理する
        
        // 5. 要素を入れ替える
        // std::swapはTokenIDsがstd::vector<int>などのコンテナであれば自動的に使えます
        std::swap(p_Keyword[index_a], p_Keyword[index_b]);
    }

    return p_Keyword;
}

// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | Embedding : Token To ScenarioTensor
// -------------------------------------------------------------------------------------------------------------------------------------//
ScenarioTensor yycgCore::helperEmbeddingTokenToScenarioTensor( TokenIDs p_Keyword )
{
    ScenarioTensor result_tensor;
    
    // 💡 乱数生成器の準備（m_RandomGenerator はクラスメンバと仮定）
    std::mt19937 t_RandomGenerator( static_cast<unsigned int>(std::time(nullptr)) );
    
    // D[1] の分布: E_DECORATE_TIMES の範囲 (0 から 4)
    constexpr int TimeMaxID = (int)E_DECORATE_TIMES::LIST_SIZE - 1;
    std::uniform_int_distribution<> time_distribution( 0 , TimeMaxID );

    // 💡 D[2] の分布: E_DECORATE_INTENSITY の範囲 (0 から 4)
    constexpr int IntensityMaxID = (int)E_DECORATE_INTENSITY::LIST_SIZE - 1;
    std::uniform_int_distribution<> intensity_distribution( 0 , IntensityMaxID );

    // 💡 D[3] の分布: E_DECORATE_INTENSITY の範囲 (0 から 4)
    constexpr int Tmp1MaxID = (int)E_DECORATE_TMP1::LIST_SIZE - 1;
    std::uniform_int_distribution<> tmp1_distribution( 0 , Tmp1MaxID );

    // 💡 D[4] の分布: E_DECORATE_INTENSITY の範囲 (0 から 4)
    constexpr int Tmp2MaxID = (int)E_DECORATE_TMP2::LIST_SIZE - 1;
    std::uniform_int_distribution<> tmp2_distribution( 0 , Tmp2MaxID );

    // 💡 D[5] の分布: E_DECORATE_INTENSITY の範囲 (0 から 4)
    constexpr int Tmp3MaxID = (int)E_DECORATE_TMP3::LIST_SIZE - 1;
    std::uniform_int_distribution<> tmp3_distribution( 0 , Tmp3MaxID );

    // 💡 D[6] の分布: E_DECORATE_INTENSITY の範囲 (0 から 4)
    constexpr int Tmp4MaxID = (int)E_DECORATE_TMP4::LIST_SIZE - 1;
    std::uniform_int_distribution<> tmp4_distribution( 0 , Tmp4MaxID );

    // 💡 D[7] の分布: E_DECORATE_INTENSITY の範囲 (0 から 4)
    constexpr int Tmp5MaxID = (int)E_DECORATE_TMP5::LIST_SIZE - 1;
    std::uniform_int_distribution<> tmp5_distribution( 0 , Tmp5MaxID );

    // 💡 D[8] の分布: E_DECORATE_INTENSITY の範囲 (0 から 4)
    constexpr int Tmp6MaxID = (int)E_DECORATE_TMP6::LIST_SIZE - 1;
    std::uniform_int_distribution<> tmp6_distribution( 0 , Tmp6MaxID );

    // 💡 D[9] の分布: E_DECORATE_INTENSITY の範囲 (0 から 4)
    constexpr int Tmp7MaxID = (int)E_DECORATE_TMP7::LIST_SIZE - 1;
    std::uniform_int_distribution<> tmp7_distribution( 0 , Tmp7MaxID );

    int current_excitement_value = 0;

    // トークンIDのリストをループ処理します
    for (int token_id : p_Keyword)
    {
        // 1. 元のトークンID (N) を埋め込みの最初の要素 (D[0]) として追加します
        result_tensor.push_back(token_id);
        
        // D[1]: 装飾時間 ID (ランダムに埋め込み済み)
        int random_time_id = time_distribution( t_RandomGenerator );
        result_tensor.push_back( random_time_id ); 
        
        // 🚨 修正箇所：D[2] にランダムな E_DECORATE_INTENSITY ID を埋め込む
        int random_intensity_id = intensity_distribution( t_RandomGenerator );
        result_tensor.push_back( random_intensity_id ); // D[2] : ランダムな強度ID
        
        int random_tmp1_id = tmp1_distribution( t_RandomGenerator );
        result_tensor.push_back(random_tmp1_id); // D[3]

        int random_tmp2_id = tmp2_distribution( t_RandomGenerator );
        result_tensor.push_back(random_tmp2_id); // D[4]

        int random_tmp3_id = tmp3_distribution( t_RandomGenerator );
        result_tensor.push_back(random_tmp3_id); // D[5]

        int random_tmp4_id = tmp4_distribution( t_RandomGenerator );
        result_tensor.push_back(random_tmp4_id); // D[6]

        int random_tmp5_id = tmp5_distribution( t_RandomGenerator );
        result_tensor.push_back(random_tmp5_id); // D[7]

        int random_tmp6_id = tmp6_distribution( t_RandomGenerator );
        result_tensor.push_back(random_tmp6_id); // D[8]

        int random_tmp7_id = tmp7_distribution( t_RandomGenerator );
        result_tensor.push_back(random_tmp7_id); // D[9]

        int action_value = YYCG::KEYWORD_TABLE[token_id].extimental; 
        current_excitement_value += action_value;

        if( current_excitement_value <= 0 ){ current_excitement_value = 0;}
        result_tensor.push_back( current_excitement_value ); 

        result_tensor.push_back(0); // D[11]
        result_tensor.push_back(0); // D[12]
        result_tensor.push_back(0); // D[13]
        result_tensor.push_back(0); // D[14]
        result_tensor.push_back(0); // D[15]
    }

    // N * D(16) の埋め込みが完了した ScenarioTensor を返します
    return result_tensor;

}

// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | Helper Method | Embedding : ScenarioTensor To AdditionalTransitionTensor
// -------------------------------------------------------------------------------------------------------------------------------------//
ScenarioTensor yycgCore::helperAdditionalScenarioTensor( ScenarioTensor p_tensor )
{
    // N * D の元のテンソル要素数
    size_t original_size = p_tensor.size();
    if (original_size == 0 || original_size % EmbeddingDimention != 0) {
        // エラー処理（ここでは省略）
        return {}; 
    }

    // 元のトークン数 N
    size_t N = original_size / EmbeddingDimention;

    // 補完後の (2N - 1) * D のテンソル
    // m_AdditionalTransitionTensor ではなく、ローカルで生成して返す
    ScenarioTensor result_tensor;
    
    // 元のテンソル p_tensor のインデックス i は、トークン i の開始位置 i * D を示す

    // N個のActionノードと (N-1)個のTransitionノードを生成
    for (size_t i = 0; i < N; ++i)
    {
        // ----------------------------------------------------
        // 1. Action ノードのコピー (奇数インデックス: 1, 3, 5...)
        // ----------------------------------------------------
        
        // p_tensor から i 番目のトークン (Action) をそのままコピー
        for (int d = 0; d < EmbeddingDimention; ++d)
        {
            // 元の D[d] の値をそのままコピー
            result_tensor.push_back(p_tensor[i * EmbeddingDimention + d]);
        }
        
        // ----------------------------------------------------
        // 2. Transition ノードの生成 (偶数インデックス: 2, 4, 6...)
        // ----------------------------------------------------
        
        // 最後のActionの後はTransitionを生成しない (N-1個のTransition)
        if (i < N - 1) 
        {
            // 次のトークン (Action) の開始位置
            size_t next_idx = (i + 1) * EmbeddingDimention;
            
            for (int d = 0; d < EmbeddingDimention; ++d)
            {
                int prev_val = p_tensor[i * EmbeddingDimention + d];
                int next_val = p_tensor[next_idx + d];
                
                // 💡 純粋な足し合わせ（平均）
                // [x] = ([x+1] + [x-1]) / 2 を計算し、intに丸めます
                int mid_val;
                
                if (d == 0) {
                    //mid_val = static_cast<int>(std::round( (prev_val + next_val) / 2.0 ));
                    mid_val = static_cast<int>(std::round( prev_val + next_val));
                } else {
                    // D[1] 以降の要素
                    mid_val = static_cast<int>(std::round( (prev_val + next_val) / 2.0 ));
                    //mid_val = static_cast<int>(std::round( prev_val + next_val));
                }
                
                result_tensor.push_back(mid_val);
            }
        }
    }
    
    // 生成された (2N-1) * D のテンソルを返します
    return result_tensor;
}

// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | TokenID Keyword Log
// -------------------------------------------------------------------------------------------------------------------------------------//
yycgCore& yycgCore::helperLogKeyword( [[maybe_unused]] TokenIDs p_Keyword ) //It's a hassle to expand in the future
{
    std::cout << "KeywordIDs: [";

    if( !p_Keyword.empty() )
    {
        std::cout << p_Keyword[0];
        for (size_t i = 1; i < p_Keyword.size(); ++i){ std::cout << ", " << p_Keyword[i]; }
    }
    std::cout << "]" << std::endl;

    return *this;
}

// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | Keyword Text Log
// -------------------------------------------------------------------------------------------------------------------------------------//

yycgCore& yycgCore::helperLogText( [[maybe_unused]] TokenText p_Keyword ) //It's a hassle to expand in the future
{
    std::cout << "KeywordText: [";

    if( !p_Keyword.empty() )
    {
        std::cout << p_Keyword[0];
        for (size_t i = 1; i < p_Keyword.size(); ++i){ std::cout << std::setw(3) << ", " << p_Keyword[i]; }
    }
    std::cout << "]" << std::endl;

    return *this;
}

// -------------------------------------------------------------------------------------------------------------------------------------//
// Helper Method | Scenario Vector Log
// -------------------------------------------------------------------------------------------------------------------------------------//
yycgCore& yycgCore::helperScenarioLog( [[maybe_unused]] ScenarioTensor p_Scenario )
{
    std::cout << "ScenarioTensor (N * D(16)):" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    
    const int D_SIZE = 16; 
    
    for (size_t i = 0; i < p_Scenario.size(); ++i)
    {
        if (i % D_SIZE == 0)
        {
            if (i != 0)
            {
                std::cout << std::endl;
            }
            std::cout << std::setw(2) << "[" << i / D_SIZE << "] "; 
        }
        
        std::cout << std::setw(3) << p_Scenario[i];
        if ( (i + 1) % D_SIZE != 0)
        {
            std::cout << ", ";
        }
    }
    
    if (!p_Scenario.empty())
    {
        std::cout << std::endl;
    }
    
    std::cout << "--------------------------------------------------------" << std::endl;

    return *this;
}