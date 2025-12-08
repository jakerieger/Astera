# NthEngine Build and Install Script for Windows
# This script automates the process of building and installing NthEngine on Windows

param(
    [switch]$Clean = $false,
    [ValidateSet('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')]
    [string]$BuildType = 'Release',
    [string]$InstallPrefix = '',
    [string]$BuildDir = 'build',
    [switch]$Install = $false,
    [switch]$Uninstall = $false,
    [ValidateSet('all', 'Runtime', 'Development', 'Tools')]
    [string]$Component = 'all',
    [ValidateSet('', 'NSIS', 'ZIP')]
    [string]$Package = '',
    [switch]$Help = $false
)

function Write-ColorText {
    param(
        [string]$Text,
        [string]$Color = 'White'
    )
    Write-Host $Text -ForegroundColor $Color
}

function Write-Info {
    param([string]$Message)
    Write-ColorText "[INFO] $Message" -Color Green
}

function Write-Warning {
    param([string]$Message)
    Write-ColorText "[WARNING] $Message" -Color Yellow
}

function Write-Error {
    param([string]$Message)
    Write-ColorText "[ERROR] $Message" -Color Red
}

function Show-Help {
    @"
NthEngine Build and Install Script for Windows

Usage: .\build-and-install.ps1 [OPTIONS]

Options:
    -Help               Show this help message
    -Clean              Clean build directory before building
    -BuildType TYPE     Build type: Debug, Release, RelWithDebInfo (default: Release)
    -InstallPrefix PATH Installation prefix (default: C:\Program Files\NthEngine)
    -BuildDir DIR       Build directory (default: build)
    -Install            Install after building
    -Uninstall          Uninstall from previous installation
    -Component COMP     Install specific component: Runtime, Development, Tools, all (default: all)
    -Package TYPE       Create package: NSIS, ZIP (requires CPack and NSIS for installer)

Examples:
    # Build in release mode
    .\build-and-install.ps1 -BuildType Release

    # Build and install to custom location
    .\build-and-install.ps1 -BuildType Release -InstallPrefix "C:\NthEngine" -Install

    # Clean build and install (requires admin)
    .\build-and-install.ps1 -Clean -BuildType Release -Install

    # Install only runtime component
    .\build-and-install.ps1 -Install -Component Runtime

    # Create NSIS installer
    .\build-and-install.ps1 -Package NSIS

    # Uninstall
    .\build-and-install.ps1 -Uninstall

Note: Installation to Program Files requires running PowerShell as Administrator
"@
    exit 0
}

function Test-Administrator {
    $currentUser = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($currentUser)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

# Show help if requested
if ($Help) {
    Show-Help
}

# Handle uninstall
if ($Uninstall) {
    Write-Info "Uninstalling NthEngine..."
    
    $manifestPath = Join-Path $BuildDir "install_manifest.txt"
    if (-not (Test-Path $manifestPath)) {
        Write-Error "install_manifest.txt not found in $BuildDir"
        Write-Error "Cannot proceed with uninstallation"
        exit 1
    }
    
    if (-not (Test-Administrator)) {
        Write-Warning "Uninstallation may require administrator privileges"
    }
    
    Get-Content $manifestPath | ForEach-Object {
        if (Test-Path $_) {
            Write-Info "Removing: $_"
            Remove-Item $_ -Force -ErrorAction SilentlyContinue
        }
    }
    
    Write-Info "Uninstallation complete"
    exit 0
}

# Check for admin rights if installing to default location
if ($Install -and [string]::IsNullOrEmpty($InstallPrefix)) {
    if (-not (Test-Administrator)) {
        Write-Warning "Installing to Program Files requires administrator privileges"
        Write-Warning "Please run PowerShell as Administrator or specify a custom prefix with -InstallPrefix"
        exit 1
    }
}

# Clean build directory if requested
if ($Clean) {
    Write-Info "Cleaning build directory: $BuildDir"
    if (Test-Path $BuildDir) {
        Remove-Item $BuildDir -Recurse -Force
    }
}

# Create build directory
Write-Info "Creating build directory: $BuildDir"
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
Push-Location $BuildDir

try {
    # Configure with CMake
    Write-Info "Configuring with CMake (Build Type: $BuildType)..."
    
    $cmakeArgs = @("-DCMAKE_BUILD_TYPE=$BuildType")
    
    if (-not [string]::IsNullOrEmpty($InstallPrefix)) {
        $cmakeArgs += "-DCMAKE_INSTALL_PREFIX=$InstallPrefix"
        Write-Info "Install prefix: $InstallPrefix"
    } else {
        Write-Info "Install prefix: C:\Program Files\NthEngine (default)"
    }
    
    # Add Visual Studio generator if available
    $cmakeArgs += ".."
    
    & cmake @cmakeArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed"
        exit 1
    }
    
    # Build
    Write-Info "Building NthEngine..."
    & cmake --build . --config $BuildType --parallel
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed"
        exit 1
    }
    
    Write-Info "Build completed successfully"
    
    # Create package if requested
    if (-not [string]::IsNullOrEmpty($Package)) {
        Write-Info "Creating $Package package..."
        & cpack -G $Package -C $BuildType
        if ($LASTEXITCODE -ne 0) {
            Write-Error "Package creation failed"
            exit 1
        }
        
        Write-Info "Package created successfully"
        Get-ChildItem -Filter "*.exe","*.zip" | ForEach-Object {
            Write-Info "  Created: $($_.FullName)"
        }
    }
    
    # Install if requested
    if ($Install) {
        Write-Info "Installing NthEngine..."
        
        if (-not (Test-Administrator) -and [string]::IsNullOrEmpty($InstallPrefix)) {
            Write-Warning "Installation requires administrator privileges"
            exit 1
        }
        
        $installArgs = @("--config", $BuildType)
        
        if ($Component -ne 'all') {
            $installArgs += @("--component", $Component)
            Write-Info "Installing component: $Component"
        }
        
        & cmake --install . @installArgs
        if ($LASTEXITCODE -ne 0) {
            Write-Error "Installation failed"
            exit 1
        }
        
        Write-Info "Installation completed successfully"
    }
    
    Write-Info "All operations completed successfully!"
    Write-Info ""
    Write-Info "Next steps:"
    
    if ($Install) {
        Write-Host "  - The 'nth.exe' tool is now installed"
        Write-Host "  - Add the bin directory to your PATH to use it from anywhere"
        Write-Host "  - Include NthEngine in your CMake projects with: find_package(NthEngine REQUIRED)"
    } else {
        Write-Host "  - Run with -Install flag to install: .\build-and-install.ps1 -Install"
        Write-Host "  - Or manually install: cd $BuildDir; cmake --install . --config $BuildType"
    }
    
} finally {
    Pop-Location
}
