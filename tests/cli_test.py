"""End-to-end checks; run after make all on Linux with Python 3."""
import pathlib
import random
import re
import subprocess
import tempfile

root = pathlib.Path(__file__).resolve().parents[1]
rng = random.Random(1114)
commands = []
active = []
for _ in range(3000):
    if active and rng.random() < 0.48:
        commands.append("dealloc")
        active.pop()
    else:
        size = rng.randint(1, 512)
        commands.append(f"alloc: {size}")
        active.append(size)

with tempfile.TemporaryDirectory() as directory:
    datafile = pathlib.Path(directory) / "datafile"
    for strategy in ("firstfit", "bestfit", "quickfit"):
        executable = str(root / strategy)
        assert subprocess.run([executable], capture_output=True).returncode != 0
        assert subprocess.run([executable, str(datafile) + ".missing"],
                              capture_output=True).returncode != 0
        for invalid in ("alloc: 0", "alloc: 513", "alloc: -1",
                        "alloc: -18446744073709551615", "alloc: 1 extra",
                        "alloc: 1.5", "alloc:", "dealloc extra", "unknown"):
            datafile.write_text(invalid + "\n")
            result = subprocess.run([executable, str(datafile)], capture_output=True)
            assert result.returncode != 0, (strategy, invalid)
        datafile.write_text("\n".join(commands) + "\n")
        result = subprocess.run([executable, str(datafile)], text=True,
                                capture_output=True, check=True)
        assert result.stdout.count("Allocated list:") == 1
        occupied = result.stdout.split("Free list")[0]
        records = re.findall(r"Address: (\S+), Size: (\d+), Used: (\d+)", occupied)
        assert [int(used) for _, _, used in records] == active
        assert len({address for address, _, _ in records}) == len(records)
        for _, size, used in records:
            assert int(size) in (32, 64, 128, 256, 512)
            assert 0 < int(used) <= int(size)
        if strategy == "quickfit":
            for size in (32, 64, 128, 256, 512):
                assert f"Partition {size}:" in result.stdout
print("CLI checks passed for all three strategies (3000 operations each).")
