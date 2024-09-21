#include "board.h"
#include <immintrin.h>
#include <vector>
#include <ranges>
#include <sstream>

static const std::pair<int, int> kKingMoves[] = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

static const std::pair<int, int> kRookDirections[] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}};

static const std::pair<int, int> kBishopDirections[] = {
    {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};


// Which squares can rook attack from every of squares.
static const square kRookAttacks[] = {
    0x01010101010101FEULL, 0x02020202020202FDULL, 0x04040404040404FBULL,
    0x08080808080808F7ULL, 0x10101010101010EFULL, 0x20202020202020DFULL,
    0x40404040404040BFULL, 0x808080808080807FULL, 0x010101010101FE01ULL,
    0x020202020202FD02ULL, 0x040404040404FB04ULL, 0x080808080808F708ULL,
    0x101010101010EF10ULL, 0x202020202020DF20ULL, 0x404040404040BF40ULL,
    0x8080808080807F80ULL, 0x0101010101FE0101ULL, 0x0202020202FD0202ULL,
    0x0404040404FB0404ULL, 0x0808080808F70808ULL, 0x1010101010EF1010ULL,
    0x2020202020DF2020ULL, 0x4040404040BF4040ULL, 0x80808080807F8080ULL,
    0x01010101FE010101ULL, 0x02020202FD020202ULL, 0x04040404FB040404ULL,
    0x08080808F7080808ULL, 0x10101010EF101010ULL, 0x20202020DF202020ULL,
    0x40404040BF404040ULL, 0x808080807F808080ULL, 0x010101FE01010101ULL,
    0x020202FD02020202ULL, 0x040404FB04040404ULL, 0x080808F708080808ULL,
    0x101010EF10101010ULL, 0x202020DF20202020ULL, 0x404040BF40404040ULL,
    0x8080807F80808080ULL, 0x0101FE0101010101ULL, 0x0202FD0202020202ULL,
    0x0404FB0404040404ULL, 0x0808F70808080808ULL, 0x1010EF1010101010ULL,
    0x2020DF2020202020ULL, 0x4040BF4040404040ULL, 0x80807F8080808080ULL,
    0x01FE010101010101ULL, 0x02FD020202020202ULL, 0x04FB040404040404ULL,
    0x08F7080808080808ULL, 0x10EF101010101010ULL, 0x20DF202020202020ULL,
    0x40BF404040404040ULL, 0x807F808080808080ULL, 0xFE01010101010101ULL,
    0xFD02020202020202ULL, 0xFB04040404040404ULL, 0xF708080808080808ULL,
    0xEF10101010101010ULL, 0xDF20202020202020ULL, 0xBF40404040404040ULL,
    0x7F80808080808080ULL};
// Which squares can bishop attack.
static const square kBishopAttacks[] = {
    0x8040201008040200ULL, 0x0080402010080500ULL, 0x0000804020110A00ULL,
    0x0000008041221400ULL, 0x0000000182442800ULL, 0x0000010204885000ULL,
    0x000102040810A000ULL, 0x0102040810204000ULL, 0x4020100804020002ULL,
    0x8040201008050005ULL, 0x00804020110A000AULL, 0x0000804122140014ULL,
    0x0000018244280028ULL, 0x0001020488500050ULL, 0x0102040810A000A0ULL,
    0x0204081020400040ULL, 0x2010080402000204ULL, 0x4020100805000508ULL,
    0x804020110A000A11ULL, 0x0080412214001422ULL, 0x0001824428002844ULL,
    0x0102048850005088ULL, 0x02040810A000A010ULL, 0x0408102040004020ULL,
    0x1008040200020408ULL, 0x2010080500050810ULL, 0x4020110A000A1120ULL,
    0x8041221400142241ULL, 0x0182442800284482ULL, 0x0204885000508804ULL,
    0x040810A000A01008ULL, 0x0810204000402010ULL, 0x0804020002040810ULL,
    0x1008050005081020ULL, 0x20110A000A112040ULL, 0x4122140014224180ULL,
    0x8244280028448201ULL, 0x0488500050880402ULL, 0x0810A000A0100804ULL,
    0x1020400040201008ULL, 0x0402000204081020ULL, 0x0805000508102040ULL,
    0x110A000A11204080ULL, 0x2214001422418000ULL, 0x4428002844820100ULL,
    0x8850005088040201ULL, 0x10A000A010080402ULL, 0x2040004020100804ULL,
    0x0200020408102040ULL, 0x0500050810204080ULL, 0x0A000A1120408000ULL,
    0x1400142241800000ULL, 0x2800284482010000ULL, 0x5000508804020100ULL,
    0xA000A01008040201ULL, 0x4000402010080402ULL, 0x0002040810204080ULL,
    0x0005081020408000ULL, 0x000A112040800000ULL, 0x0014224180000000ULL,
    0x0028448201000000ULL, 0x0050880402010000ULL, 0x00A0100804020100ULL,
    0x0040201008040201ULL};
// Which squares can knight attack.
static const square kKnightAttacks[] = {
    0x0000000000020400ULL, 0x0000000000050800ULL, 0x00000000000A1100ULL,
    0x0000000000142200ULL, 0x0000000000284400ULL, 0x0000000000508800ULL,
    0x0000000000A01000ULL, 0x0000000000402000ULL, 0x0000000002040004ULL,
    0x0000000005080008ULL, 0x000000000A110011ULL, 0x0000000014220022ULL,
    0x0000000028440044ULL, 0x0000000050880088ULL, 0x00000000A0100010ULL,
    0x0000000040200020ULL, 0x0000000204000402ULL, 0x0000000508000805ULL,
    0x0000000A1100110AULL, 0x0000001422002214ULL, 0x0000002844004428ULL,
    0x0000005088008850ULL, 0x000000A0100010A0ULL, 0x0000004020002040ULL,
    0x0000020400040200ULL, 0x0000050800080500ULL, 0x00000A1100110A00ULL,
    0x0000142200221400ULL, 0x0000284400442800ULL, 0x0000508800885000ULL,
    0x0000A0100010A000ULL, 0x0000402000204000ULL, 0x0002040004020000ULL,
    0x0005080008050000ULL, 0x000A1100110A0000ULL, 0x0014220022140000ULL,
    0x0028440044280000ULL, 0x0050880088500000ULL, 0x00A0100010A00000ULL,
    0x0040200020400000ULL, 0x0204000402000000ULL, 0x0508000805000000ULL,
    0x0A1100110A000000ULL, 0x1422002214000000ULL, 0x2844004428000000ULL,
    0x5088008850000000ULL, 0xA0100010A0000000ULL, 0x4020002040000000ULL,
    0x0400040200000000ULL, 0x0800080500000000ULL, 0x1100110A00000000ULL,
    0x2200221400000000ULL, 0x4400442800000000ULL, 0x8800885000000000ULL,
    0x100010A000000000ULL, 0x2000204000000000ULL, 0x0004020000000000ULL,
    0x0008050000000000ULL, 0x00110A0000000000ULL, 0x0022140000000000ULL,
    0x0044280000000000ULL, 0x0088500000000000ULL, 0x0010A00000000000ULL,
    0x0020400000000000ULL};
// Opponent pawn attacks
static const square kPawnAttacks[] = {
    0x0000000000000200ULL, 0x0000000000000500ULL, 0x0000000000000A00ULL,
    0x0000000000001400ULL, 0x0000000000002800ULL, 0x0000000000005000ULL,
    0x000000000000A000ULL, 0x0000000000004000ULL, 0x0000000000020000ULL,
    0x0000000000050000ULL, 0x00000000000A0000ULL, 0x0000000000140000ULL,
    0x0000000000280000ULL, 0x0000000000500000ULL, 0x0000000000A00000ULL,
    0x0000000000400000ULL, 0x0000000002000000ULL, 0x0000000005000000ULL,
    0x000000000A000000ULL, 0x0000000014000000ULL, 0x0000000028000000ULL,
    0x0000000050000000ULL, 0x00000000A0000000ULL, 0x0000000040000000ULL,
    0x0000000200000000ULL, 0x0000000500000000ULL, 0x0000000A00000000ULL,
    0x0000001400000000ULL, 0x0000002800000000ULL, 0x0000005000000000ULL,
    0x000000A000000000ULL, 0x0000004000000000ULL, 0x0000020000000000ULL,
    0x0000050000000000ULL, 0x00000A0000000000ULL, 0x0000140000000000ULL,
    0x0000280000000000ULL, 0x0000500000000000ULL, 0x0000A00000000000ULL,
    0x0000400000000000ULL, 0x0002000000000000ULL, 0x0005000000000000ULL,
    0x000A000000000000ULL, 0x0014000000000000ULL, 0x0028000000000000ULL,
    0x0050000000000000ULL, 0x00A0000000000000ULL, 0x0040000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    0x0000000000000000ULL};


struct MagicParams {
    square mask;
    Square* attacks_table;
};

static MagicParams rook_magic_params[64];
static MagicParams bishop_magic_params[64];


static Square rook_attacks_table[102400];
static Square bishop_attacks_table[5248];


static void BuildAttacksTable(MagicParams* magic_params, Square* attacks_table, const std::pair<int, int>* directions) {
    int table_offset = 0;

    for (auto b_sq = 0; b_sq < 64; b_sq++) {
        square mask = 0;

        for (int j = 0; j < 4; j++) {
            auto direction = directions[j];
            auto dst_row = BoardSquare(b_sq).row();
            auto dst_col = BoardSquare(b_sq).col();

            while (true) {
                dst_row += direction.first;
                dst_col += direction.second;

                if (dst_row >= 0 && dst_row < 8 && dst_col >= 0 && dst_col < 8) {
                    const i_square dst_sq = dst_row * 8 + dst_col;
                    mask |= (1 << dst_sq);
                } else {
                    break;
                }
            }
        }

        magic_params[b_sq].mask = mask;

        std::vector<BoardSquare> occupancy_squares;

        for (auto occ_sq: Square(magic_params[b_sq].mask)) {
            occupancy_squares.emplace_back(occ_sq);
        }


        magic_params[b_sq].attacks_table = &attacks_table[table_offset];

        for (int i = 0; i < (1 << occupancy_squares.size()); i++) {
            attacks_table[table_offset + i] = 0;
        }


        for (int i = 0; i < (1 << occupancy_squares.size()); i++) {
            Square occupancy(0);

            for (size_t bit = 0; bit < occupancy_squares.size(); bit++) {
                occupancy.set_if(occupancy_squares[bit], (1 << bit) & i);
            }


            Square attacks(0);

            for (int j = 0; j < 4; j++) {
                auto direction = directions[j];
                auto dst_row = b_sq / 8;
                auto dst_col = b_sq % 8;
                while (true) {
                    dst_row += direction.first;
                    dst_col += direction.second;

                    if (dst_row >= 0 && dst_row < 8 && dst_col >= 0 &&
                        dst_col < 8) {
                        const BoardSquare destination(dst_row, dst_col);
                        attacks.set(destination);
                        if (occupancy.get(destination)) break;
                    } else {
                        break;
                    }
                }
            }


            square index = _pext_u64(occupancy.as_int(), magic_params[b_sq].mask);

            attacks_table[table_offset + index] = attacks;
        }

        table_offset += (1 << occupancy_squares.size());
    }
}


static inline Square GetRookAttacks(const square rook_sq, const square occupied) {
    square index = _pext_u64(occupied, rook_magic_params[rook_sq].mask);
    return rook_magic_params[rook_sq].attacks_table[index];
}
static inline Square GetBishopAttacks(const square bishop_sq, const square occupied) {
    square index = _pext_u64(occupied, bishop_magic_params[bishop_sq].mask);
    return bishop_magic_params[bishop_sq].attacks_table[index];
}

void InitializeMagicBitboards() {
    BuildAttacksTable(rook_magic_params, rook_attacks_table, kRookDirections);
    BuildAttacksTable(bishop_magic_params, bishop_attacks_table, kBishopDirections);
}


MoveList DuckBoard::GenerateMoves() const {
    MoveList result;
    result.reserve(20 * 64);
    for (auto source: us_) {
        if (source == us_king_) {
            for (const auto& delta: kKingMoves) {
                const auto dst_row = source.row() + delta.first;
                const auto dst_col = source.col() + delta.second;
                const std::optional<BoardSquare> m_dest = BoardSquare::make(dst_row, dst_col);

                if (!m_dest.has_value()) {
                    continue;
                }

                const BoardSquare destination = m_dest.value();
                if (us_.get(destination) || duck_ == destination) continue;

                for (auto duck: duck_after_move(source, destination))
                    result.emplace_back(source, destination, duck);
            }
        }


        bool processed_piece = false;

        if (rooks_.get(source)) {
            processed_piece = true;
            Square attacked = GetRookAttacks(source.as_square(), occupied().as_square()) - us_ - duck_;


            for (const auto& destination: attacked) {
                for (auto duck: duck_after_move(source, destination))
                    result.emplace_back(source, destination, duck);
            }
        }

        if (bishops_.get(source)) {
            processed_piece = true;
            Square attacked = GetBishopAttacks(source.as_square(), occupied().as_square()) - us_ - duck_;


            for (const auto& destination: attacked) {
                for (auto duck: duck_after_move(source, destination))
                    result.emplace_back(source, destination, duck);
            }
        }

        if (processed_piece) continue;

        if (pawns_.get(source)) {
            // forward
            {
                const auto dst_row = source.row() + 1;
                const auto dst_col = source.col();
                const BoardSquare destination(dst_row, dst_col);

                if (!occupied().get(destination)) {
                    if (dst_row != 7) {
                        for (auto duck : duck_after_move(source, destination))
                            result.emplace_back(source, destination, duck);
                        if (dst_row == 2) {
                            if (!occupied().get(3)) {
                                for (auto duck : duck_after_move(source, destination))
                                    result.emplace_back(source, destination,
                                                        duck);
                            }
                        }
                    } else {
                        for (auto duck : duck_after_move(source, destination))
                            result.emplace_back(source, destination, duck,
                                                Move::Promotion::Queen);
                    }
                }
            }

            // captures
            {
                for (auto direction : { -1, 1 }) {
                    const auto dst_row = source.row() + 1;
                    const auto dst_col = source.col() + direction;
                    const std::optional<BoardSquare> m_dest =
                        BoardSquare::make(dst_row, dst_col);

                    if (!m_dest.has_value()) {
                        continue;
                    }
                    const BoardSquare destination = m_dest.value();

                    if (them_.get(destination)) {
                        if (dst_row == 7) {
                            for (auto duck : duck_after_move(source, destination))
                                result.emplace_back(source, destination, duck,
                                                    Move::Promotion::Queen);
                        } else {
                            for (auto duck : duck_after_move(source, destination))
                                result.emplace_back(source, destination, duck);
                        }
                    }
                }
            }
            continue;
        }

        // knight
        {
            for (const auto destination: kKnightAttacks[source.as_int()] - us_ - duck_) {
                for (auto duck : duck_after_move(source, destination))
                    result.emplace_back(source, destination, duck);
            }
        }
    }
    return result;
}


bool DuckBoard::ApplyMove(Move move) {
    auto from = move.from();
    auto to = move.to();

    us_.reset(from);
    us_.set(to);

    bool reset_50_moves = them_.get(to);
    them_.reset(to);
    rooks_.reset(to);
    bishops_.reset(to);
    pawns_.reset(to);


    reset_50_moves |= pawns_.get(from);

    if (from == us_king_) {
        us_king_ = to;
        return reset_50_moves;
    }

    if (to.row() == 7 && pawns_.get(from)) {
        rooks_.set(to);
        bishops_.set(to);
        pawns_.reset(from);
        return true;
    }
    

    rooks_.set_if(to, rooks_.get(from));
    bishops_.set_if(to, bishops_.get(from));
    pawns_.set_if(to, pawns_.get(from));
    rooks_.reset(from);
    bishops_.reset(from);
    pawns_.reset(from);

    return reset_50_moves;
}

void DuckBoard::Clear() {
    *this = DuckBoard();
}

void DuckBoard::Mirror() {
    us_.Mirror();
    them_.Mirror();
    std::swap(us_, them_);
    rooks_.Mirror();
    bishops_.Mirror();
    pawns_.Mirror();
    us_king_.Mirror();
    them_king_.Mirror();
    std::swap(us_king_, them_king_);
    duck_.Mirror();
    flipped_ = !flipped_;
}

const char* DuckBoard::kStartposFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";

void DuckBoard::SetFromFen(std::string fen) {
    Clear();
    int row = 7;
    int col = 0;

    std::istringstream fen_str(fen);
    std::string board;
    fen_str >> board;
    std::string turn = "w";
    fen_str >> turn;

    for (char c: board) {
        if (c == '/') {
            --row;
            col = 0;
            continue;
        }
        if (std::isdigit(c)) {
            col += c - '0';
            continue;
        }
        if (c != 'd') {
            if (std::isupper(c)) {
                us_.set(row, col);
            } else {
                them_.set(row, col);
            }
        }

        if (c == 'd') {
            duck_.set(row, col);
            has_duck_ = true;
        } else if (c == 'K') {
            us_king_.set(row, col);
        } else if (c == 'k') {
            them_king_.set(row, col);
        } else if(c == 'R' || c == 'r') {
            rooks_.set(row, col);
        } else if (c == 'B' || c == 'b') {
            bishops_.set(row, col);
        } else if (c == 'Q' || c == 'q') {
            rooks_.set(row, col);
            bishops_.set(row, col);
        } else if (c == 'P' || c == 'p') {
            pawns_.set(row, col);
        } else if (c == 'N' || c == 'n') {

        }
        ++ col;
    }

    if (turn == "b") {
        Mirror();
    }
}


std::string DuckBoard::DebugString() const {
    std::string result;

    result += '\n';
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            if (has_duck_ && duck_ == BoardSquare(i, j)) {
                result += 'd';
            } else if (!us_.get(i, j) && !them_.get(i, j)) {
                result += '.';
            } else if (has_us_king() && us_king_ == i * 8 + j) {
                result += 'K';
            } else if (has_them_king() && them_king_ == i * 8 + j) {
                result += 'k';
            } else {
                char c = '?';
                if (pawns_.get(i, j)) {
                    c = 'p';
                } else if (bishops_.get(i, j)) {
                    if (rooks_.get(i, j)) {
                        c = 'q';
                    } else {
                        c = 'b';
                    }
                } else if (rooks_.get(i, j)) {
                    c = 'r';
                } else {
                    c = 'n';
                }
                if (us_.get(i, j)) c = std::toupper(c);
                result += c;
            }
        }
        if (i == 0) {
            result +=
                flipped_ ? " (from black's eyes)" : " (from white's eyes)";
        }
        result += '\n';
    }
    return result;
}