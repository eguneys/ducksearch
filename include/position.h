#include "board.h"
class Position {
    public:
    Position(const Position&parent, Move m);
    Position(const DuckBoard& board, int rule50_ply, int game_ply);


    uint64_t Hash() const;
    bool IsBlackToMove() const { return us_board_.flipped(); }

    int GetGamePly() const { return ply_count_; }

    int GetRepetitions() const { return repetitions_; }

    int GetPliesSincePrevRepetition() const { return cycle_length_; }

    void SetRepetitions(int repetitions, int cycle_length) {
        repetitions_ = repetitions;
        cycle_length_ = cycle_length;
    }

    int GetRule50Ply() const { return rule50_ply_; }


    const DuckBoard& GetBoard() const { return us_board_; }
    const DuckBoard& GetThemBoard() const { return them_board_; }

    const DuckBoard& GetWhiteBoard() const { return us_board_.flipped() ? them_board_ : us_board_; }

std::string DebugString() const;

private:
DuckBoard us_board_;
DuckBoard them_board_;

int rule50_ply_ = 0;
int repetitions_;
int cycle_length_;
int ply_count_ = 0;
};

std::string GetFen(const Position& pos);

enum class GameResult : uint8_t { UNDECIDED, BLACK_WON, DRAW, WHITE_WON };
GameResult operator-(const GameResult& res);

class PositionHistory {
    public:
    PositionHistory() = default;


    const Position& Starting() const { return positions_.front(); }

    const Position& Last() const { return positions_.back(); }

    const Position& GetPositionAt(int idx) const { return positions_[idx]; }


    void Reserve(int size) { positions_.reserve(size); }

    int GetLength() const { return positions_.size(); }


    void Reset(const DuckBoard& board, int rule50_ply, int game_ply);

    void Append(Move m);

    void Pop() { positions_.pop_back(); }


    GameResult ComputeGameResult() const;

    bool IsBlackToMove() const { return Last().IsBlackToMove(); }


    uint64_t HashLast(int positions) const;

    bool DidRepeatSinceLastZeroingMove() const;

    private:
    int ComputeLastMoveRepetitions(int* cycle_length) const;

    std::vector<Position> positions_;
};