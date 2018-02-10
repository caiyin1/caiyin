#ifndef __ARCHIVE_MACRO_H__
#define __ARCHIVE_MACRO_H__

/*
 * 读取变量名宏
*/

// 游戏复盘信息存储读取的key
#define GAME_RECORD_NAME "558e720aa36b6ab76287b05a1a6df694"

// 复盘信息加密的key
#define JSON_KEY "ddce90d1aca98ac5a19e32ff81bf7e59"
// 文件名
#define JSON_FILE_NAME "myhero.json"
// 玩家得分
#define JSON_PLAYER_SCORE "playerScore"
// 子弹数总数
#define JSON_BOMB_SIZE "bombSize"
// 正方形块块生成概率
#define JSON_CUBE_PROBABILITY "cubeProbability"
// 三角形块块生成概率
#define JSON_TRIANGLE_PROBABILITY "triangleProbability"
// 八边形块块生成概率
#define JSON_OCTAGON_PROBABILITY "octagonProbability"
// 黑洞块块生成概率
#define JSON_BLACK_HOLE_PROBABILITY "blackHoleProbability"
// 块块生命值
#define JSON_BLOCK_HP "blockHp"
// 本局得分
#define JSON_ROUND_SCORE "roundScore"
// 本局得分等级
#define JSON_ROUND_SCORE_LEVEL "roundScoreLevel"
// 游戏状态
#define JSON_GAME_STATUS "gameStatus"

// 游戏每局时间
#define JSON_GAME_ROUND_TIME "roundTime"

// 块块变量名
#define JSON_BLOCK_DATA "blockData"
// 状态
#define JSON_BLOCK_TYPE "type"
// 位置
#define JSON_BLOCK_POINT "point"
// x轴位置
#define JSON_BLOCK_POINT_X "posX"
// y轴位置
#define JSON_BLOCK_POINT_Y "posY"

// 食物数据
#define  JSON_FOOD_DATA "foodData"
#define JSON_FOOD_TYPE "foodType"
#define JSON_FOOD_POSX "posX"
#define JSON_FOOD_POSY "posY"
// 飞行子弹数据
#define JSON_FLY_BOMB_DATA "flyBombData"
#define JSON_FLY_BOMB_POSX "posX"
#define JSON_FLY_BOMB_POSY "posY"
#define JSON_FLY_BOMB_SPEEDX "speedX"
#define JSON_FLY_BOMB_SPEEDY "speedY"
// 飞行中的子弹和未发射的子弹(用来)
#define JSON_FLY_BOMB_SIZE "flyBombSize"
// 子弹发射时的速度
#define JSON_SHOOT_BOMB_SPEED "shootBombSpeed"
// 已经发射的子弹
#define JSON_ALREADY_SHOOT_BOMB_NUM "alreadyShootBombNum"
// 炮台x轴位置
#define JSON_FORT_POSX "fortPosX"
// 已经生成的食物
#define JSON_ALREADY_GENERATED_FOOD_SIZE "alreadyGeneratedFoodSize"

/*
 * @breif 内存保护宏
*/
// 子弹数量的key
#define SAFE_BOMB_SIZE_KEY "f12d363f79f60c7e36f00170e23f6586"
// 玩家得分的key
#define SAFE_PLAYER_SOCRE_KEY "45a4a7e0d0753098a200f2e489c2499e"
// 历史最高分
#define SAFE_HIGHEST_SCORE "dbe12860b149baebba41e572e4afe785"
// 本局得分
#define SAFE_ROUND_SCORE "cdc22cc01c7e21f857119f5428fca164"


#endif // !__ARCHIVE_MACRO_H__
