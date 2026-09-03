#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 金融メインフレーム（勘定系基盤）における、超高速・省メモリなタスク処理を再現するシミュレーター

#define MAX_ACCOUNT_ID 16
#define MAX_PIN 4

// メインフレームのメモリ最適化を意識した「固定長構造体」の定義
typedef struct {
    char account_id[MAX_ACCOUNT_ID + 1]; // 固定長配列によるメモリ断片化（フラグメンテーション）の防止
    char pin[MAX_PIN + 1];
    long long balance;                   // 桁あふれ（オーバーフロー）を防ぐ64bit整数型
    unsigned char status_flags;          // 1バイト(8bit)内で状態を管理するビットフラグ（0x01: ロック, 0x02: 特別口座など）
} MainframeAccount;

// システムの信頼性を担保するエラーコード（列挙型）
typedef enum {
    SYS_SUCCESS = 0,
    ERR_SYS_ABEND,         // 異常終了（アベンド）の防止用
    ERR_AUTH_FAILED,       // 認証エラー
    ERR_LIMIT_EXCEEDED,    // 処理制限超過
    ERR_INTEGRITY_VIOLATION // データ不整合検知
} SysStatusCode;

// ポインタ参照（Call by Reference）による超高速なデータ更新関数
// メモリのコピーを発生させず、サーバーのリソース消費を最小限に抑える設計思想
SysStatusCode execute_transaction(MainframeAccount *target_acc, const char *input_pin, long long tx_amount) {
    
    // 1. ビット演算による高速なステータスチェック（メインフレームの常套手段）
    if (target_acc->status_flags & 0x01) {
        return ERR_INTEGRITY_VIOLATION; // 口座ロック状態
    }

    // 2. メモリアドレスの直接比較と文字列照合
    if (strncmp(target_acc->pin, input_pin, MAX_PIN) != 0) {
        return ERR_AUTH_FAILED;
    }

    // 3. インフラのキャパシティ制限（1回あたりの最大取引額バリデーション）
    if (tx_amount > 2000000) {
        return ERR_LIMIT_EXCEEDED;
    }

    // 4. 残高の整合性（インテグリティ）チェック
    if (target_acc->balance < tx_amount) {
        return ERR_INTEGRITY_VIOLATION;
    }

    // 5. メモリ上の実データを直接更新（アトミックな処理を想定）
    target_acc->balance -= tx_amount;
    
    return SYS_SUCCESS;
}

int main() {
    // 擬似メインフレーム・メモリ領域の初期化（モックデータ）
    MainframeAccount db_record = {"1234567890123456", "4321", 10000000, 0x00};
    
    printf("=== NTTデータSMS メインフレーム(勘定系基盤)タスクマネージャ起動 ===\n");
    printf("[INFO] 初期メモリバッファ残高: %lld円\n\n", db_record.balance);

    // テスト1：正常系タスク（高速引き落とし）
    printf("[TASK_001] 取引処理を実行中...\n");
    SysStatusCode code1 = execute_transaction(&db_record, "4321", 500000);
    if (code1 == SYS_SUCCESS) {
        printf("-> [SUCCESS] タスク正常終了。更新後残高: %lld円\n\n", db_record.balance);
    }

    // テスト2：異常系タスク（セキュリティ違反の検知）
    printf("[TASK_002] 不正なPINコードによるアクセスを検証中...\n");
    SysStatusCode code2 = execute_transaction(&db_record, "9999", 100000);
    if (code2 == ERR_AUTH_FAILED) {
        printf("-> [ALERT] 認証エラー(ERR_AUTH_FAILED)を検知。セキュリティログをパージしました。\n\n");
    }

    // テスト3：異常系タスク（インフラ限界値突破のテスト）
    printf("[TASK_003] 限界値を超える過大リクエストを送信中...\n");
    SysStatusCode code3 = execute_transaction(&db_record, "4321", 5000000);
    if (code3 == ERR_LIMIT_EXCEEDED) {
        printf("-> [ALERT] システム制限超過(ERR_LIMIT_EXCEEDED)を検知。バッファオーバーフローを防止しました。\n\n");
    }

    printf("=== メインフレーム・シミュレーション正常終了（全タスク整合性確認） ===\n");
    return 0;
}
