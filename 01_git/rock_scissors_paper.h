

enum GameValue {Rock, Scissors, Paper, GameValue_Last};
enum GameResult {Win, Draw, Lose};

void printf_md5(const char* const message, const enum GameValue value);
const char* const to_string(const enum GameValue value);
enum GameResult get_game_result(const enum GameValue player, const enum GameValue computer);
enum GameValue generate_computer_move();
