Name:    privilege-checker
Summary: Privilege Management
Version: 0.0.4
Release: 0
Group:   System/Libraries
License: SAMSUNG
Source0: %{name}-%{version}.tar.gz

BuildRequires: cmake
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(libwbxml2)

%if "%{_repository}" == "wearable"
BuildRequires: pkgconfig(db-util)
%endif


%if 0%{?tizen_profile_wearable}
%description
Privilege Management

%package -n privilege-checker-devel
summary: privilege-checker server
Group: Development/Libraries
Requires: privilege-checker = %{version}-%{release}

%description -n privilege-checker-devel
privilege-checker devel

%package -n capi-security-privilege-manager
Summary:    Privilege Manager API
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  gettext-tools
BuildRequires:  pkgconfig(pkgmgr-info)

%description -n capi-security-privilege-manager
The Privilege Manager API provides functions to get information about privilege information of installed packages.

%package  -n capi-security-privilege-manager-devel
Summary:  Privilege Manager API (Development)
Group:    TO_BE/FILLED_IN
Requires:	capi-security-privilege-manager

%description -n capi-security-privilege-manager-devel
The Privilege Manager API provides functions to get information about privilege information of installed packages.(DEV)

%prep
%setup -q

%build
#############################################
echo "TIZEN_PROFILE_WEARABLE"
echo "TIZEN_PROFILE_WEARABLE"
echo "TIZEN_PROFILE_WEARABLE"
cp -R modules_wearable/* ./
#############################################
#%{!?build_type:%define build_type "Release"}

%if 0%{?tizen_build_binary_release_type_eng}
export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"
%endif

echo cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DDPL_LOG="ON" 
cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DDPL_LOG="ON" 
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2 %{buildroot}/usr/share/license/privilege-checker
mkdir -p %{buildroot}/opt/dbspace
cp util/res/opt/dbspace/.privilegelist.db /%{buildroot}/opt/dbspace/

%make_install

%files -n privilege-checker
/usr/share/license/privilege-checker
/usr/bin/*
/opt/dbspace/.privilegelist.db
%manifest packaging/privilege-checker.manifest

%files -n capi-security-privilege-manager
%{_libdir}/libcapi-security-privilege-manager.so*
/usr/share/locale/*
%manifest packaging/capi-security-privilege-manager.manifest

%files -n capi-security-privilege-manager-devel
%{_includedir}/privilegemgr/*.h

%clean
rm -rf %{buildroot}



%else


%description
Privilege Management

%package -n privilege-checker-devel
summary: privilege-checker server
Group: Development/Libraries
Requires: privilege-checker = %{version}-%{release}

%description -n privilege-checker-devel                                          
privilege-checker devel

%package -n capi-security-privilege-manager
Summary:    Privilege Manager API
Group:      TO_BE/FILLED_IN
License:    Apache-2.0
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  gettext-tools
BuildRequires:  pkgconfig(pkgmgr-info)

%description -n capi-security-privilege-manager
The Privilege Manager API provides functions to get information about privilege information of installed packages.
%package  -n capi-security-privilege-manager-devel
Summary:  Privilege Manager API (Development)
Group:    TO_BE/FILLED_IN
Requires:   capi-security-privilege-manager

%description -n capi-security-privilege-manager-devel
The Privilege Manager API provides functions to get information about privilege information of installed packages.(DEV)

%prep
%setup -q

%build
#############################################
echo "TIZEN_PROFILE_MOBILE"
echo "TIZEN_PROFILE_MOBILE"
echo "TIZEN_PROFILE_MOBILE"
cp -R modules_mobile/* ./
#############################################
#%{!?build_type:%define build_type "Release"}
echo cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DDPL_LOG="ON" 
cmake . -DPREFIX=%{_prefix} \
        -DEXEC_PREFIX=%{_exec_prefix} \
        -DLIBDIR=%{_libdir} \
        -DINCLUDEDIR=%{_includedir} \
        -DCMAKE_BUILD_TYPE=%{build_type} \
        -DVERSION=%{version} \
        -DDPL_LOG="ON" 
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2 %{buildroot}/usr/share/license/privilege-checker
cp LICENSE.APLv2 %{buildroot}/usr/share/license/capi-security-privilege-manager
cp LICENSE.APLv2 %{buildroot}/usr/share/license/capi-security-privilege-manager-devel
mkdir -p %{buildroot}/opt/dbspace
cp util/res/opt/dbspace/.privilegelist.db /%{buildroot}/opt/dbspace/

%make_install

%files -n privilege-checker
/usr/share/license/privilege-checker
/usr/bin/*
/opt/dbspace/.privilegelist.db
%manifest packaging/privilege-checker.manifest

%files -n capi-security-privilege-manager
%{_libdir}/libcapi-security-privilege-manager.so*
/usr/share/locale/*
%manifest packaging/capi-security-privilege-manager.manifest
/usr/share/license/capi-security-privilege-manager

%files -n capi-security-privilege-manager-devel
%{_includedir}/privilegemgr/*.h
/usr/share/license/capi-security-privilege-manager-devel

%clean
rm -rf %{buildroot}
                      
%endif

