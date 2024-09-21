#include "position.h"


Position::Position(const Position& parent, Move m) : rule50_ply_(parent.rule50_ply_ + 1), ply_count_(parent.ply_count_ + 1) {
    them_board_ = parent.us_board_;
    const bool is_zeroing = them_board_.ApplyMove(m);
    us_board_ = them_board_;
    us_board_.Mirror();
    if (is_zeroing) rule50_ply_ = 0;
}


Position::Position(const DuckBoard& board, int rule50_ply, int game_ply)
: rule50_ply_(rule50_ply), repetitions_(0), ply_count_(game_ply) {
    us_board_ = board;
    them_board_ = board;
    them_board_.Mirror();
}

uint64_t Position::Hash() const {
    return HashCat({us_board_.Hash(), static_cast<unsigned long>(repetitions_)});
}


std::string Position::DebugString() const { return us_board_.DebugString(); }

GameResult operator-(const GameResult& res) {
    return res == GameResult::BLACK_WON   ? GameResult::WHITE_WON
           : res == GameResult::WHITE_WON ? GameResult::BLACK_WON
                                          : res;
}

GameResult PositionHistory::ComputeGameResult() const {
    const auto& board = Last().GetBoard();
    auto legal_moves = board.GenerateMoves();
    if (legal_moves.empty()) {
        if (!board.has_us_king()) {
            return IsBlackToMove() ? GameResult::WHITE_WON : GameResult::BLACK_WON;
        } else {
            return IsBlackToMove() ? GameResult::BLACK_WON : GameResult::WHITE_WON;
        }
    }
    if (Last().GetRule50Ply() >= 100) return GameResult::DRAW;
    if (Last().GetRepetitions() >= 2) return GameResult::DRAW;

    return GameResult::UNDECIDED;
}


void PositionHistory::Reset(const DuckBoard& board, int rule50_ply,
int game_ply) {
    positions_.clear();
    positions_.emplace_back(board, rule50_ply, game_ply);
}

void PositionHistory::Append(Move m) {
    positions_.push_back(Position(Last(), m));

    int cycle_length;
    int repetitions = ComputeLastMoveRepetitions(&cycle_length);
    positions_.back().SetRepetitions(repetitions, cycle_length);
}


int PositionHistory::ComputeLastMoveRepetitions(int* cycle_length) const {
    *cycle_length = 0;
    const auto& last = positions_.back();
    if (last.GetRule50Ply() < 4) return 0;

    for (int idx = positions_.size() - 3; idx >= 0; idx -= 2) {
        const auto& pos = positions_[idx];
        if (pos.GetBoard() == last.GetBoard()) {
            *cycle_length = positions_.size() - 1 - idx;
            return 1 + pos.GetRepetitions();
        }
        if (pos.GetRule50Ply() < 2) return 0;
    }
    return 0;
}


bool PositionHistory::DidRepeatSinceLastZeroingMove() const {
    for (auto iter = positions_.rbegin(), end = positions_.rend(); iter != end;
    ++iter) {
        if (iter->GetRepetitions() > 0) return true;
        if (iter->GetRule50Ply() == 0) return false;
    }
    return false;
}

uint64_t PositionHistory::HashLast(int positions) const {
    uint64_t hash = positions;
    for (auto iter = positions_.rbegin(), end = positions_.rend(); iter != end;
         ++iter) {
            if (!positions--) break;
            hash = HashCat(hash, iter->Hash());
    }
    return HashCat(hash, Last().GetRule50Ply());
}

char GetPieceAt(const DuckBoard&board, int row, int col) {
    char c= '\0';

    if (board.duck().has_value() && board.duck().value() == BoardSquare(row, col)) {
        c = 'd';
    } else if (board.ours().get(row, col) || board.theirs().get(row, col)) {
        if (board.pawns().get(row, col)) {
            c = 'P';
        } else if (board.kings().get(row, col)) {
            c = 'K';
        } else if (board.bishops().get(row, col)) {
            c = 'B';
        } else if (board.queens().get(row, col)) {
            c = 'Q';
        } else if (board.rooks().get(row, col)) {
            c = 'R';
        } else {
            c = 'N';
        }
        if (board.theirs().get(row, col)) {
            c = std::tolower(c);
        }
    }
    return c;
}

std::string GetFen(const Position& pos) {
    std::string result;

    const DuckBoard& board = pos.GetWhiteBoard();
    for (int row = 7; row >= 0; --row) {
        int emptycounter = 0;
        for (int col = 0; col < 8; ++col) {
            char piece = GetPieceAt(board, row, col);
            if (emptycounter > 0 && piece) {
                result += std::to_string(emptycounter);
                emptycounter = 0;
            }
            if (piece) {
                result += piece;
            } else {
                emptycounter++;
            }
        }
        if (emptycounter > 0) result += std::to_string(emptycounter);
        if (row > 0) result += '/';
    }
    result += pos.IsBlackToMove() ? " b" : " w";
    result += " " + std::to_string(pos.GetRule50Ply());
    result += " " + std::to_string(
        (pos.GetGamePly() + (pos.IsBlackToMove() ? 1 : 2)) / 2);
    return result;
}