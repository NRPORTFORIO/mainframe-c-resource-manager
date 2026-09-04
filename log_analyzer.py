import os
import re
from datetime import datetime

# 擬似的なC言語シミュレーターのログファイル（テスト用）
LOG_FILE = "mainframe_system.log"

def create_dummy_log():
    """C言語シミュレーターが出力するようなログを擬似的に生成（実機がない場合用）"""
    dummy_logs = [
        "[2026-09-04 14:00:01] [INFO] Account: 1234-5678-9012 - Deposit: 10,000 JPY - Success",
        "[2026-09-04 14:05:22] [WARNING] Account: 9999-9999-9999 - BitFlag: Account Locked (0x01)",
        "[2026-09-04 14:10:15] [INFO] Account: 5555-4444-3333 - Withdraw: 5,000 JPY - Success",
        "[2026-09-04 14:15:00] [CRITICAL] Memory Buffer Near Limit (92% usage) - Risk of ABEND",
        "[2026-09-04 14:20:30] [ERROR] Account: 1234-5678-9012 - Insufficient Balance - Failed",
    ]
    with open(LOG_FILE, "w", encoding="utf-8") as f:
        f.write("\n".join(dummy_logs))

def analyze_logs():
    """Pythonによるログの自動解析・アベンド兆候の検知"""
    if not os.path.exists(LOG_FILE):
        create_dummy_log()  # ログがなければ自動生成
        
    print("=" * 60)
    print(" NTT DATA SMS - Infrastructure Log Analyzer (Python Version) ")
    print("=" * 60)
    
    warning_count = 0
    error_count = 0
    critical_count = 0
    
    with open(LOG_FILE, "r", encoding="utf-8") as f:
        for line in f:
            # ログの重要度（レベル）をチェック
            if "[WARNING]" in line:
                warning_count += 1
                print(f"⚠️ [検知] アカウントロックを検出: {line.strip()}")
            elif "[ERROR]" in line:
                error_count += 1
                print(f"❌ [検知] 取引エラーを検出: {line.strip()}")
            elif "[CRITICAL]" in line:
                critical_count += 1
                print(f"🚨 [警告] システムアベンド（異常終了）の兆候を検出!!: {line.strip()}")

    print("-" * 60)
    print("【解析結果サマリー】")
    print(f" - WARNING (注意) : {warning_count} 件")
    print(f" - ERROR   (異常) : {error_count} 件")
    print(f" - CRITICAL(致命) : {critical_count} 件")
    
    if critical_count > 0:
        print("\n🔥 [アクション] 自動アラート：即時運用保守チームへのエスカレーションが必要です。")
    print("=" * 60)

if __name__ == "__main__":
    analyze_logs()
