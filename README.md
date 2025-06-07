# 🔥 WPMHook_x64/x86 - The Silent Payload Snatcher 🔥

> *"Get that motha' fucka'"*  

---

## 📖 **About**  
This is a lean, mean DLL designed to **intercept and dump payloads** mid-injection using `WriteProcessMemory` hooks.  

- **Original concept by**: [un4ckn0wl3z](https://github.com/un4ckn0wl3z/wpm-hook)  
- **MinHook by**: [Tsuda Kageyu](https://github.com/TsudaKageyu/minhook)
- **Maintained/adapted by**: 0xLUC1F3R (2025)  

Built for **reverse engineers**, **malware analysts**, and **curious coders** who want to see what’s being injected into processes—**without disrupting the injection itself**.  

---

## 🛠 **How It Works**  
- Injects `wpmhook_x64.dll` (or `wpmhook_x86.dll`) into the **injector process** (not the target).  
- Hooks `WriteProcessMemory` and **logs every buffer** written into another process.  
- Dumps **raw binary payloads** (`dump_0.bin`, `dump_1.bin`, etc.) to disk.  
- Writes a **detailed log** (`WriteProcessMemoryLog.txt`) with:  
  - Buffer addresses  
  - Payload sizes  
  - Hex previews of the data  
  - Timestamps (implicitly via file writes)  

---

## 🚀 **Why Use This?**  
✅ **Extract embedded payloads** from manual-mapped injectors.  
✅ **Analyze cheats/malware** that hide their injection logic.  
✅ **Debug injection techniques** without crashing the target.  
✅ **No interference**—just **silent logging & dumping**.  

---

## 🧰 **How To Use**  
1. **Build it** (or grab the prebuilt DLLs):  
   - Open `.sln` in **Visual Studio 2022**.  
   - Set to **Release** mode.  
   - Build for **x64** or **x86** (depending on the injector’s bitness).  
   - Output: `wpmhook_x64.dll` or `wpmhook_x86.dll`.  

2. **Inject it** into the **injector process** (not the target!).  
   - Use your favorite injector (`Process Hacker`, manual mapping, etc.).  

3. **Let the injector run**—it’ll dump every `WriteProcessMemory` call.  

4. **Check the output**:  
   - `dump_*.bin` → Raw payloads.  
   - `WriteProcessMemoryLog.txt` → Full execution log.  

---

## 🔍 **Example Log Output**  
```plaintext
=== WriteProcessMemory Hooked ===  
hProcess: 0x00000ABC  
lpBaseAddress: 0x7FFE0300  
lpBuffer: 0x0012FED4  
nSize: 0x400  
Buffer dumped to dump_0.bin  
Dump Size: 0x400  
Buffer Content (Hex): 4D 5A 90 00 03 00 00 00...  
===============================
``` 
⚠ Disclaimer
For legal/educational use only.
Don’t be a skid. Don’t steal shit. Don’t blame me if you break something.
If you use this to rip malware/cheats, respect the scene—don’t be a fuckwad.

📜 Credits & License
Original WPM Hook: un4ckn0wl3z

MinHook: Tsuda Kageyu

Modifications: 0xLUC1F3R (2025)

License: Unlicensed (Do whatever, but credit where due.)

💡 Pro Tip: Use a hex editor (like HxD) or a PE analyzer (PE-bear) to inspect the dumped .bin files.

Happy ~~hacking~~ researching! 🍻
