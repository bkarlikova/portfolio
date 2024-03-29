#### _english version_
___
## Gameplay:

The user selects from the main menu: **Start a new game**, **Quit game**, or **Continue game** if there is a game in progress. If the user chooses to start a new game, the game is loaded from a configuration file. If there are missing or extra important pieces of information in the configuration file necessary for the game, the program throws an exception and terminates. It is not possible to configure the game from an invalid file under any circumstances.

Next, the user creates a character from the available character options. Characters differ in effects, maximum attribute values, and health points. After selecting a character, the user allocates the assigned units among defined attributes. Then, they choose a weapon, armor, and three items.

The gameplay is simulated on a finite deterministic automaton, which is assuredly deterministic due to automatic control during game loading. The user moves through states, encountering either a battle with any NPC character, from which they gain XP upon defeating the monster, or a riddle or question they must answer. If answered correctly, they must choose one of the offered products. If the user's 3rd attribute value is high enough as a randomly generated number in this state, they obtain a bonus mysterious item.

Next, they are presented with a menu of moves to other states, only to states they have not yet visited. If they reach a state marked as the final state, or if their health points drop to 0, the game ends successfully or unsuccessfully for the user.

At any time, the user can return to the menu by typing `m` or `menu`, where they can quit the game, start a new one, continue a previously played game if one exists, or save the current game to any file.

If a game is already in progress, the user can choose to save the game from the menu using the command `sv` or `save` to save the current game state. Then, they can choose to continue from the menu using `c` or `continue` to resume playing.

___

#### Configuration Files:

The game is configured from six configuration files. It's important to adhere to their naming conventions.

- `attributes.json`
  This file defines 4 attributes under IDs 1 - 4 and their textual names.
- `automaton.json`
  This file defines a finite deterministic automaton with exactly one initial state and any number of final states. However, the initial state must not be final. Every state must be reachable, and in the base configuration, every non-final state must have a transition to any other state. The automaton does not allow transitions to the initial state.
- `effects.json`
  All effects occurring in the game are defined in this file. They are divided into permanent effects, which affect the user throughout the entire game, and temporary effects, which only affect the user in a given state. Upon moving to another state, the user's modified attributes return to their original values.
- `items.json`
  This file defines all items that may appear in the game, including weapons, armor, and miscellaneous objects. Each item has a unique ID and name. Specifically, a weapon has a hit point value, armor has anti-hit points, and an object has one existing effect, either temporary or permanent.
- `npcs.json`
  This file stores all characters that may appear in battles. They have their own ID, name, defined attributes, weapon, armor, and objects they can use instead of a weapon against the player in a battle.
- `usersChar.json`
  If this file is in the `config_file` directory, it contains available characters for the user. Characters have defined IDs, names, effects, which are only permanent and affect the character throughout the game, health points, and maximum attribute values. However, if this file is in any directory within the `saved_games` directory, it contains only one character with set attribute values, a user-defined name, along with a weapon, armor, and items the user had at the time of saving the game.

##### If any essential data is missing from or in logical inconsistency with the game implementation in any of these files, the program throws an exception and prevents the game from starting.

If the user wants to display an exception and determine in which function it was thrown and in which it was gradually caught, the `-DDEBUG` flag is provided for compilation. Simply compile using `make DEBUG=true`.
___

#### Testing:

A set of twelve tests is prepared, either simulating users or merely verifying functionality within the game. To run the tests, the `-DTEST` flag is prepared, so with the command `make TEST=true run`, only tests are run, not the game.
___

#### Polymorphism:

- The room system is implemented as a DFA with states represented by the abstract class `CState`, or its children `Event`, a room for any event, knowledge question, item acquisition, and `Fight`, where a clash with one instance of the class `NonPlayingChar` occurs. After completing an action in `State`, the player moves to the next room using a transitional command. Polymorphic methods exist over the `State` class, e.g., `run_state`, which handles an entire cycle in one state.
- The abstract class `Item` has 3 children: `Armour`, `Weapon`, `Object`. Users can add these items to their inventory, which is implemented as `Inventory`, with the `m_items` variable represented as `vector<shared_ptr<Item>>`. Polymorphic functions are called here to check the child's data type, print, or create a copy.
- Characters in the game are represented by the abstract class `Character`, from which `User` and `NonPlayingCharacter` inherit. Among the pure virtual methods are, for example, `use_object`, which uses an object on a character, distinguishing for User whether the effect is temporary or permanent, while for NPC it is not, as the NPC survives only for the duration of one fight and thus does not need to save and restore its attributes.

___

#### _česká verze_

## Gamebook

___

### Zadání z Progtestu:

Engine musí implementovat následující funkcionality:\

- Umožněte vytvořit herní postavu, kde si uživatel může
    - navolit alespoň 3 různé druhy atributů (př. síla, obratnost, inteligence, …),
    - vybrat schopnost/i nebo povolání (př. válečník, lučištník, mág, …).
- Každá schopnost (povolání) může mít více efektů, které mohou být aktivní (uživatel je musí použít) nebo pasivní (jsou
  aktivovány automaticky).
- Interakce se světem probíhá pomocí textového popisu jednotlivých místností a zadáváním akcí prostřednictvím
  klávesnice (př. volba akce z předem definované nabídky, klíčová slova ala Zork nebo Albion, …).
- Implementujte jednoduchý soubojový systém a inventář
- Hra musí být konfigurovatelná ze souboru:
    - definice atributů (př. název, …) a schopností/povolání (př. název, cena, efekty, …)
    - definice světa (př. místnosti, předměty, příšery, dialogy, cíl, …)
    - Hra musí umožnit uložit a následně načíst rozehranou hru.

Kde lze využít polymorfismus?:

- soubojové entity: postava, příšera, stroj (př. past, bomba, …)
- efekty schopností/povolání: válečník (útočí dvakrát za kolo), lučištník (může střílet na větší vzdálenost), mág (nižší
  síla, ale každé kolo automaticky zraňuje nepřátele v dosahu), … (jde o efekty těchto schopnosti/povolání, které se
  dají kombinovat, nikoli o schopnosti/povolání samotné)
- efekty akcí: souboje, události (pád ze stromu, …), přechod do jiné místnosti, …
- inventář: postava (ruce, pouze 2 věci + brnění), batoh (nosnost), toulec (pouze šípy), kapsy (pouze malé věci), …
- uživatelské rozhraní: konzole, ncurses, SDL (ke každé místnosti může být obrázek, souboje můžou být vizualizované),
  OpenGL (různé varianty), …

___

## Vlastní implementace:

#### Průběh hry:

Uživatel si vybere ze zákládního menu: **Začít novou hru**, **Ukončit hru** nebo **Pokračovat ve hře** pokud již nějaká
hra je rozehraná.Pokud uživatel vybere novou hru, načte se hra z konfiguračního souboru. Pokud v konfiguračním souborou
chybí nebo přebývají nějaké informace důležité pro průběh hry, program vyhodí vyjímku a ukončí běh. Hru není v žádném
případě možné nakonfigurovat z nevalidního souboru.

Dale si uživatel vytvoří postavu z nabízené nabídky postav. Postavy se mezi sebou liší se efety, maximálními hodnotami
atributů a hodnotou zdraví. Po zvolení postavy, uživatel rozdělí přidělené jednotky mezi definované attributy. Následně
si vybere zbraň, brnění a tři předměty.

Průběh hry je simulovaný na konečném deterministickém automatu, který je jistě deterministický díky automatické kontorle
při načítání hry. Uživatel se pohybuje po stavech, ve kterých je pro něj připraven souboj s libovolnou NPC postavou, ve
kterém po poražení příšery získá příslušný počet XP, nebo hádanka, či otázka na kterou musí odpovědět. Pokud odpoví
správně musí si vybrat jeden z nabízených produktů. Pokud uživatelova hodnota 3. atributu je dostatečně vysoká jako
náhodně vygenerované číslo v tomto stavu, získává bonudový tajemný předmět.

Následně se mu zobrazí nabídka přesunů do dalších stavů, pouze do stavů, ve kterých ještě nebyl. Pokud dorazí do stavu
označeného jako koncový stav, nebo jeho hodnota zdraví klesne na 0 hra pro uživatele úspěšně resp. núspěšně končí.

Uživatel se kdykoli může vrátit příkazem `m` nebo `menu` do menu, ve kterém může hru ukončit, začít novou, žačít nějakou
rozehranou pokud taková hra existuje a nebo uložit stávající hru do libovolného souboru.

Pokud již běží hra uživatel může v menu zvolit uložení hry pomocí příkazu `sv` nebo `save` a si uložit aktuální stav
hry. Následně opět v menu zvolit pokračování pomocá `c` nebo `continue` a pokračovat ve hře.

___

#### Konfigurační soubory:

Hra se konfiguruje ze šesti konfiguračních souborů. Jejich název je důležité dodržovat.

- `attributes.json`
  V tomto souboru jsou zadefinované 4 atributy pod id 1 - 4 a jejich textová jména
- `automaton.json`
  V tomto souboru je zadefinovaný konečný deterministický automat z právě jední počátečním stavem a libovolným počtem
  koncových stavů. Ovšem počáteční stav nesmí být koncový. Každý stav musí být dosažitelný a v základní konfiguraci i
  každý nekoncový stav musí mít přechod do libovolného dalšího stavu. Automat nepovoluje přechody do počátečního stavu.
- `effects.json`
  Veškéré efekty vyskytující se ve hře jsou zadefinované právě v tomto souboru. Dělí se na permanentní, ty uživatele
  ovlivní po celý zbytek hry, a na dočasné, které ovlivní uživatele jenom v daném stavu, po přesunu do dalšího stavu se
  jeho změněné atributy vrátí na původní hodnoty.
- `items.jdon`
  V tomto souboru jsou definované všechny věci, které se moho uve hře vyskytnour. Zbraně, brnění i libovolné objekty.
  Každá věc ma unikátní id a libovolné jméno, konkrétně pak zbraň má hodnotu *hit points*, brnění má *anti hit points* a
  nakonec objekt má jeden existující efect, dočasný či permanenntí.
- `npc_s.json`
  V tomto souboru jsou uloženy všechny postavy, které se mohou objevit v soubojích. Mají vlastní id, jméno, definované
  atributy, zbraň, brnění a objekty, které mohou použít místo zbraně proti hráči v souboji.
- `usersChar.jons`
  Pokud je tento soubor v adresáři `config_file`, tak obsahuje nabízené postavy pro uživatele. Postavy mají definované
  id, jméno, efekty, které jsou jen permanentní a ovlivňují postavu po celou dobu hry, hodnotu zdraví a maximální
  hodnoty 4 atributů.
  Pokud je ovšem tento soubor v libovolném z adresářů v adresáři `saved_games`, obsahuje pouze jednu postavu s
  nastavenými hodnotami atributů, libovolným jménem nastaveným uživatelem se zbraní, brněním a věcmi, které měl uživatel
  ve chvíli kdy ukládal hru.

##### Pokud v některém ze souborů chybí esenciální data nebo jsou v logickém nesouladu s implementací hry, program vyhodí vyjímku a neumožní hru započít.

Pokud chce uživatel vyjímku zobrazit a zjistit, ve které funkci byla vyhozena a ve kterých postupně chycena, je zde
připraven flag `-DDEBUG` pro kompilaci. Stačí zkompilovat příkazem `make DEBUG=true`
___

#### Testování

Pro testování je připravena sada dvanácti testů buď simulující uživatele nebo pouze ověřující funkcionalitu ve hře. Pro
spuštění testů je připraven flag `-DTEST`, tedy s příkazem `make TEST=true run` se spustí pouze testy a nikoliv hra.
____

#### Polymorfismus

- systém místností je implementovaný jako DKA o stavech reprezentovaných abstraktní třídou `CState`, respektive jejími
  dětmi `Event`, místnost pro libovolnou událost,znalostní otázku, zisk předmětů a ` Fight`, ve kterém dochází ke
  střetu s jednou instancí třídy `NonPlayingChar`. Po dokončení akce v `State` se hráč dostáva pomocí přechodového
  příkazu do další místnosti. Polymorfismem jsou zde metody nad třídou `State`, např `run_state` která se stará celý
  jeden cyklus v jednom stavu.
- Abstraktní třída `Item`, má 3 děti: `Armour`,`Weapon`,`Object`. Tyto věci si uživatel může dát do inventáře, který
  je implementovaný jako `Inventory`, s proměnou `m_items` reprezentovanou jako `vector<shared_prt<Item>>`.
  Polymorficky se zde volají funkce kontrolující datový typ dítěte, výpis nebo vytvářející kopii.
- Postavy ve hře jsou reprezentované obstraktní tříou `Character`, se které dědí `User` a `NonPlayingCharacter`. Mezi
  pure virtual metody patří například `use_object`, které použije objekt na postavu s tím, že pro User rozlišuje, zda
  je effect dočasný, či permanentí zatímco u NPC nikoliv, protože ten přežívá pouze po dobu jednoho fightu a tak nemá
  potřebu ukládat a znovu obnovovat svoje atributy.
___





