# UE4 C++ 프로그래밍 입문

## 클래스 마법사 오버로딩 클래스

* BeginPlay() - 액터가 플레이 가능한 상태로 게임에 들어왔음을 알려주는 이벤트.
* Tick() - 지난번 들여온 이후의 경과된 시간만큼 프레임당 한 번씩 호출.
* Tick이 필요하지 않은 경우 성능 향상을 위해 꺼줘도 된다.

```c++
AMyActor::AMyActor()
{
    // 이 액터가 Tick() 을 매 프레임 호출하도록 설정합니다. 필요치 않은 경우 이 옵션을 끄면 퍼포먼스가 향상됩니다.
    PrimaryActorTick.bCanEverTick = true;
}
```

## 프로퍼티가 에디터에 보이도록

* 간단히 UPROPERTY(EditAnymore) 매크로를 선언 앞에 두면 된다.

```c++
    UPROPERTY(EditAnywhere)
    int32 TotalDamage;
```

* Category를 통해 다른 유사 프로퍼티와 한 섹션으로 묶을 수 있따.

```c++
    UPROPERTY(EditAnywhere, Category="Damage")
    int32 TotalDamage;
```

* 더 많은 프로퍼티 지정자는 여기서

http://api.unrealengine.com/KOR/Programming/UnrealArchitecture/Reference/Properties/Specifiers/index.html

```c++
// 예시
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    int32 TotalDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    float DamageTimeInSeconds;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category="Damage")
    float DamagePerSecond;
```

* PostInitProperties() 콜 체인에 걸어주면 디자이너 설정 값을 기반으로 기본값 생성 가능

```
void AMyActor::PostInitProperties()
{
    Super::PostInitProperties();
    DamagePerSecond = TotalDamage / DamageTimeInSeconds;
}
```

* 하지만 에디터에선 런타임으로 바꿔주는 DPS가 정상적으로 나오지 않는다.
    + 최초 로딩 프로세스에서 초기화된 이후의 값만 계산하기 때문
    + 언리얼 에디터의 런타임 변경 사항을 알려주는 이벤트가 있음.

```c++
void AMyActor::PostInitProperties()
{
    Super::PostInitProperties();

    CalculateValues();
}

void AMyActor::CalculateValues()
{
    DamagePerSecond = TotalDamage / DamageTimeInSeconds;
}

#if WITH_EDITOR
void AMyActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    CalculateValues();

    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
```