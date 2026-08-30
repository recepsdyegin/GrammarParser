import subprocess
import sys
from pathlib import Path


def run(command, cwd=None):
    print(f"> {' '.join(command)}")

    result = subprocess.run(command, cwd=cwd)

    if result.returncode != 0:
        sys.exit(result.returncode)


def main():
    project_dir = Path(__file__).parent.resolve()
    build_dir = project_dir / "test-builds"

    build_dir.mkdir(exist_ok=True)

    run(
        ["cmake", ".."],
        cwd=build_dir
    )

    run(
        ["cmake", "--build", "."],
        cwd=build_dir
    )

    run(
        ["ctest", "--output-on-failure"],
        cwd=build_dir
    )


if __name__ == "__main__":
    main()