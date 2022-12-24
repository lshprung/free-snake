Name:           simple-snake
Version:        0.1
Release:        1%{?dist}
Summary:        simple ncurses snake game
License:        Unlicense
URL:            https://github.com/lshprung/simple-snake
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc make ncurses-devel
Requires:       ncurses

%description
ncurses snake game where the only boundaries are
the terminal window.

%prep
%autosetup


%build
%configure
%make_build


%install
rm -rf $RPM_BUILD_ROOT
%make_install


%files
%{_bindir}/%{name}
%{_docdir}/%{name}/README


%changelog
* Sat Dec 24 2022 builder
- 
