# Unreal Engine 主要クラス関係図と解説

```
                                    UObject
                                       |
                 +-----------+---------+----------+---------+
                 |           |         |          |         |
              UWorld      AActor    UActorComponent  UClass  UBlueprint
                 |           |         |          
        +--------+     +-----+-----+   +-------+-------+-------+
        |              |     |     |           |       |       |
    ULevel        APawn |  AController  UStaticMeshComponent USkeletalMeshComponent USceneComponent
                   |    |     |                                       |
          +--------+    |  +--+-------+                        +-----------+
          |             |  |          |                        |           |
    ACharacter    AGameMode APlayerController AIController  USpringArmComponent UCameraComponent
```

## 主要クラスの説明

### UObject
全てのUnrealオブジェクトの基底クラス。リフレクション、ガベージコレクション、シリアル化などの基本機能を提供します。

### AActor
ゲームワールドに配置できるオブジェクトの基底クラス。位置、回転、スケールなどの空間情報を持ちます。

### UActorComponent
アクターに機能を追加するためのコンポーネントの基底クラス。アクターに関連付けられ、特定の機能を担当します。

### APawn
プレイヤーやAIによって制御可能なアクターの基底クラス。入力を受け取り、物理的にワールド内を移動できます。

### ACharacter
Pawnの拡張クラスで、2足歩行のキャラクター向けの機能を提供します。キャラクターの移動やアニメーションの基本機能を含みます。

### AController
Pawnを制御するためのクラス。AIControllerとPlayerControllerの基底クラスです。

### APlayerController
プレイヤーからの入力を処理し、対応するPawnを制御するためのクラスです。

### AIController
AIによって制御されるPawnのためのコントローラーです。AI行動を定義します。

### UWorld
ゲームのレベルを表すクラス。アクターやレベルなどのゲーム要素を管理します。

### ULevel
ゲームワールドの一部を表すクラス。複数のレベルで1つのワールドを構成できます。

### AGameMode
ゲームのルールや条件を定義するクラス。プレイヤーの参加方法、スコアリング、ゲームの勝敗条件などを管理します。

### USceneComponent
3D空間に配置されるコンポーネントの基底クラス。位置、回転、スケールなどの情報を持ちます。

### UStaticMeshComponent
静的な3Dモデルを表示するためのコンポーネントです。

### USkeletalMeshComponent
アニメーション可能な3Dモデルを表示するためのコンポーネントです。

### USpringArmComponent
カメラとキャラクターの間に「腕」を作成し、カメラの挙動を滑らかにするコンポーネントです。

### UCameraComponent
プレイヤーの視点を提供するカメラコンポーネントです。

### UClass
C++やブループリントで定義されたクラスのメタデータを保持するクラスです。

### UBlueprint
ビジュアルスクリプティングでゲームプレイ要素を作成するためのアセットです。

## クラス間の主要な関係

1. **継承関係**:
   - 全てのクラスはUObjectから継承しています
   - ACharacterはAPawnから継承しています
   - APlayerControllerとAIControllerはAControllerから継承しています

2. **所有関係**:
   - AActorは複数のUActorComponentを所有できます
   - AControllerはAPawnを制御します
   - UWorldは複数のULevelを含みます

3. **構成関係**:
   - ACharacterはUSkeletalMeshComponentを使用してキャラクターの見た目を表現します
   - UCameraComponentとUSpringArmComponentを組み合わせてプレイヤーのカメラ視点を作成します

## ゲーム開発での典型的な利用パターン

### プレイヤーキャラクターの作成
```cpp
// 基本的なキャラクタークラスの実装例
UCLASS()
class AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyCharacter();

protected:
    // カメラブーム（衝突時にカメラを引き寄せる）
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    USpringArmComponent* CameraBoom;

    // フォローカメラ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    UCameraComponent* FollowCamera;

    virtual void BeginPlay() override;
    
    // 入力イベントのバインド
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
```

### ゲームモードの設定
```cpp
// 基本的なゲームモードの実装例
UCLASS()
class AMyGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    AMyGameMode();

    virtual void BeginPlay() override;
    
    // デフォルトのポーンクラスを設定
    virtual APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot) override;
};
```
