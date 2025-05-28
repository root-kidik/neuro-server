import subprocess
import os

from dataclasses import dataclass
from pathlib import Path

@dataclass
class ProjectPaths:
    source_directory: Path
    build_directory: Path
    bin_directory: Path
    executable: Path
    log: Path

def get_project_paths() -> ProjectPaths:
    source_directory = Path(__file__).parent.parent.parent.parent
    build_directory = source_directory.joinpath('build-' + os.environ['BUILD_TYPE'])
    bin_directory = build_directory.joinpath('bin')
    executable = bin_directory.joinpath('neuro-server')
    log = bin_directory.joinpath(f'neuro-server.log') 

    return ProjectPaths(source_directory, build_directory, bin_directory, executable, log)
