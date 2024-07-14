# Using SGX to Harden Password Hashing

Intel Software Guard Extensions (SGX) allows running security-sensitive user-mode code in an 'enclave'. Enclave memory is encrypted and authenticated, making it inaccessible to code running elsewhere. Enclaves can use device-specific keys to encrypt ('seal') data for future executions of themselves or other enclaves signed with the same key.

This project implements PBKDF2 password hashing within an SGX enclave. Password hashes are accessible only to enclaves enrolled in a 'region', ensuring user passwords remain secure even if the database is compromised. GPUs are ineffective against this security measure.

A region is represented by an AES key, sealed to the enclaves in the region. The key can be kept offline and used only for enrolling new enclaves or disaster recovery.

For more information, see my [blog post](https://jbp.io/2016/01/17/using-sgx-to-hash-passwords/).

## Warning

This project is highly experimental and provided without any warranty. Use at your own risk.

The repository includes a trivial region key, the enclave signing private key, and the enclave runs in debug mode, so it offers no real security.

## Project Tour

### Interesting Files:

- **[pwenclave/pwenclave.edl](pwenclave/pwenclave.edl)**: Defines the interface between user-mode code and the enclave. An Intel-provided tool uses this definition to generate stubs for calling these functions in user-mode and converting arguments in the enclave. These stubs are found in [pwenclave/pwenclave_t.c](pwenclave/pwenclave_t.c) and [smoketest/pwenclave_u.c](smoketest/pwenclave_u.c).
- **[pwenclave/pwenclave.c](pwenclave/pwenclave.c)**: Implements the interface. Other files alongside provide PBKDF2 and additional functionality.
- **[smoketest/smoketest.c](smoketest/smoketest.c)**: Starts the enclave and tests the functions.

## Building

### Requirements:

- The [Intel SGX SDK](https://software.intel.com/en-us/sgx-sdk).
- Visual Studio 2012 (required by the SGX SDK).
- Intel SGX Platform Software (included with the SDK) and SGX-compatible hardware (e.g., a Skylake CPU and compatible BIOS). The SDK supports a simulator, though it has not been tested in this project.

This setup is Windows-only.

### Instructions:

1. Install the Intel SGX SDK and required software.
2. Load the solution in Visual Studio.
3. Hit run.

This setup was tested on a Dell Inspiron 5559 laptop.
