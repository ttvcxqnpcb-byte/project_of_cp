“以下內容為AI生成，專案內容僅供小組組內期末專題使用”

# 👻 台大電機系靈異傳說 - 視覺小說遊戲引擎

這是一個使用 C++ 和 SDL2 開發的視覺小說 (Visual Novel) 遊戲引擎核心。
本專案是我們課程的期末作業，旨在實現一個可擴充的遊戲架構，讓團隊成員可以並行開發不同的遊戲章節。

## 🎮 專案簡介

* **核心引擎**：負責圖像渲染 (LTexture)、劇情管理 (StoryManager)、音樂與音效播放 (BackgroundMusic / SoundEffect)、以及章節切換 (GameChapter)。
* **劇本格式**：使用簡單的純文字檔 (.txt) 搭配自定義標籤 (如 `[BG_school]`, `[MUSIC_stop]`) 來控制遊戲流程。
* **多型架構**：主程式透過 `GameChapter` 介面來管理不同章節，實作了低耦合的設計。

## ⚙️ 環境需求

在開始之前，請確保你的開發環境已安裝以下工具：

* **C++ 編譯器**：支援 C++14 或更高版本。
* **CMake**：版本 3.10 或更高。
* **SDL2 開發庫**：
    * SDL2
    * SDL2_image
    * SDL2_ttf
    * SDL2_mixer
* **IDE**：推薦使用 **CLion** (已配置好 CMake 整合)。

---

## 🚀 如何開始 (給組員的懶人包指南)

### 1. 下載專案

1. 點擊頁面右上方的綠色 **[Code]** 按鈕。
2. 選擇 **[Download ZIP]**。
3. 解壓縮後，使用 CLion 開啟該資料夾。

### 2. 測試運行

1.  確保 CLion 已正確載入 CMake 專案。
2.  找到 `main.cpp`，確認它引用的是模板章節：
    ```cpp
    #include "ChapterTemplate.h" // 確認這裡引用的是模板
    // ...
    gChapter = new ChapterTemplate(); // 確認這裡是 new 模板
    ```
3.  按下 **Run (執行)** 按鈕。
4.  如果一切順利，你應該會看到一個測試畫面，顯示模板劇本的內容。

---

## 🛠️ 開發指南 (如何製作你的章節)

### 步驟 1：準備素材

將你負責章節所需的素材放入 `assets` 資料夾中：
* 劇本：`assets/txt/your_chapter.txt` (請存成 UTF-8)
* 圖片：`assets/img/` (人物請依姓名分資料夾)
* 音樂/音效：`assets/music/` 或 `assets/sound/`

### 步驟 2：建立章節類別

1.  複製 `ChapterTemplate.h` 和 `ChapterTemplate.cpp`。
2.  將複製出來的檔案重新命名為你的章節名稱（例如 `ChapterInvestigate.h` / `.cpp`）。
3.  打開這兩個檔案，將程式碼中所有的 `ChapterTemplate` 替換成你的新類別名稱 (`ChapterInvestigate`)。

### 步驟 3：載入劇本

打開你的 `.cpp` 檔 (例如 `ChapterInvestigate.cpp`)，在 `load()` 函式中修改劇本路徑：

'cpp'
void ChapterInvestigate::load()
{
    // 將此處改為你的劇本檔案路徑
    if (!mStoryManager.loadScript("assets/txt/your_chapter.txt")) {
        printf("Load script failed!\n");
    }
}
步驟 4：加入編譯列表
打開 CMakeLists.txt，在 add_executable 區塊中加入你的 .cpp 檔案：

CMake

add_executable(YourGameName
    main.cpp
    # ... 其他核心檔案 ...
    ChapterInvestigate.cpp # <-- 加入這一行
)
步驟 5：在 Main 中測試
為了測試你寫的章節，請暫時修改 main.cpp：

引入你的標頭檔：#include "ChapterInvestigate.h"

修改建立章節的程式碼：gChapter = new ChapterInvestigate();

執行程式，確認你的章節運作正常。

(開發完成後，請記得將檔案 .h, .cpp, .txt 和素材圖片傳給組長整合。)

📂 檔案結構說明
.
├── assets/             # 遊戲素材 (圖片、音樂、劇本、字體)
├── BackgroundMusic.* # 背景音樂與音效管理
├── Character.* # 角色圖片管理 (繼承自 LTexture)
├── GameChapter.h       # [介面] 所有遊戲章節的基底類別
├── ChapterTemplate.* # [模板] 供組員複製使用的章節模板
├── LTexture.* # 紋理包裝類別 (處理圖片載入與渲染)
├── SoundEffect.* # 短音效管理
├── StoryManager.* # 核心劇情管理器 (解析劇本、打字機效果)
├── main.cpp            # 程式入口 (目前設定為測試模板章節)
└── CMakeLists.txt      # CMake 建置設定檔
📝 版權與致謝
本專案使用 SDL2 及其相關擴充庫。

字體使用 Zpix (最像素字體)。
