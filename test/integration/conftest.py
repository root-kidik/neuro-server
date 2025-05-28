import fcntl
import time

import pytest

from util.util import get_project_paths
from colorama import init, Fore

pytest_plugins = ['testsuite.pytest_plugin']

async def health_check(session, process):
    time.sleep(1) # Wait until service full started
    return process.poll() is None

@pytest.fixture(scope='session')
async def service_scope(create_daemon_scope):
    with open('sync.lock', 'w') as lock_file:
        init(autoreset=True)

        fcntl.flock(lock_file, fcntl.LOCK_EX)

        project_paths = get_project_paths()

        try:
            with open(project_paths.log, 'w') as log_file:
                def handle_stdout(line):
                        str = line.decode('utf-8')
                        print(Fore.RED + str)
                        log_file.write(str)

                def handle_stderr(line):
                        str = line.decode('utf-8')
                        print(Fore.RED + str)
                        log_file.write(str)

                async with create_daemon_scope(
                    args=[
                        str(project_paths.executable)
                    ],
                    stdout_handler=handle_stdout,
                    stderr_handler=handle_stderr,
                    health_check=health_check
                ) as scope:
                    yield scope
        finally:
            fcntl.flock(lock_file, fcntl.LOCK_UN)


@pytest.fixture
async def neuro_server(
    ensure_daemon_started,
    service_scope
):
    await ensure_daemon_started(service_scope)
