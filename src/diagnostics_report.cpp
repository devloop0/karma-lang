#include "../includes/diagnostics_report.hpp"

using namespace karma_lang;

namespace karma_lang {

	diagnostics_reporter::diagnostics_reporter(shared_ptr<source_token_list> stl) {
		stlist = stl;
		error_count = 0;
	}

	diagnostics_reporter::~diagnostics_reporter() {

	}

	shared_ptr<token> diagnostics_reporter::print(string message, source_token_list::iterator current_pos, karma_lang::diagnostics_reporter_kind kind) {
		if(kind == diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR)
			error_count++;
#ifdef WIN32
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		bool colored = true;
		if (handle == INVALID_HANDLE_VALUE) {
			cerr << "Could not output colored diagnostics. Defaulting to regular, non-colored output. (Perhaps you do not have the file kernel32.dll)?\n\n";
			colored = false;
		}
		if (colored) {
			CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
			GetConsoleScreenBufferInfo(handle, &console_screen_buffer_info);
			if (kind == diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR) {
				SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
				cerr << "Error ";
				SetConsoleTextAttribute(handle, console_screen_buffer_info.wAttributes);
			}
			else if (kind == diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING) {
				SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				cerr << "Warning ";
				SetConsoleTextAttribute(handle, console_screen_buffer_info.wAttributes);
			}
			else {
				SetConsoleTextAttribute(handle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				cerr << "Note ";
				SetConsoleTextAttribute(handle, console_screen_buffer_info.wAttributes);
			}
		}
		else {
			if (kind == diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR)
				cerr << "Error ";
			else if (kind == diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING)
				cerr << "Warning ";
			else
				cerr << "Note ";
		}
#else
		if(kind == diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_WARNING)
			cerr << "\033[35mWarning \033[0m";
		else if(kind == diagnostics_reporter_kind::DIAGNOSTICS_REPORTER_ERROR)
			cerr << "\033[31mError \033[0m";
		else
			cerr << "\033[34mNote \033[0m";
#endif
		int start = 0, end = 0;
		if (current_pos < stlist->end())
			start = (*current_pos)->get_column_start(), end = (*current_pos)->get_column_end() - 1;
		else
			start = -1, end = -1;
		if (current_pos < stlist->end()) {
			if (start != end)
				cerr << '[' << (*current_pos)->get_file_name() << ' ' << (*current_pos)->get_line_number() << ':' << start << '-' << end << "]: ";
			else
				cerr << '[' << (*current_pos)->get_file_name() << ' ' << (*current_pos)->get_line_number() << ':' << start << "]: ";
		}
		else {
			if (start != end)
				cerr << '[' << (*current_pos)->get_file_name() << ' ' << (*current_pos)->get_line_number() << ':' << start << '-' << end << "]: ";
			else
				cerr << "Builtin involved.";
			return nullptr;
		}
		cerr << message << "\nRegion given here for reference:\n";	
		cerr << '\t';
		int inc = 0;
		for(source_token_list::iterator it = (current_pos - 2 < stlist->begin() ? stlist->begin() : current_pos - 2); it < (current_pos + 3 >= stlist->end() ? stlist->end() : current_pos + 3); ++it) {
			if((*it)->get_token_kind() == token_kind::TOKEN_NEW_LINE)
				cerr << "new-line" << ' ';
			else
				cerr << (*it)->get_raw_string() << ' ';
			if(it < current_pos) {
				if((*it)->get_token_kind() == token_kind::TOKEN_NEW_LINE)
					inc += 8 + 1;
				else
					inc += (*it)->get_raw_string().length() + 1;
			}
		}
		cerr << '\n' << '\t';
		for(int i = 0; i < inc; i++)
			cerr << ' ';
#ifdef WIN32
		HANDLE handle2 = GetStdHandle(STD_OUTPUT_HANDLE);
		bool colored2 = true;
		if (handle2 == INVALID_HANDLE_VALUE) {
			cerr << "Could not output colored diagnostics. Defaulting to regular, non-colored output. (Perhaps you do not have the file kernel32.dll)?\n\n";
			colored2 = false;
		}
		if (colored2) {
			CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info2;
			GetConsoleScreenBufferInfo(handle2, &console_screen_buffer_info2);
			SetConsoleTextAttribute(handle2, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			cerr << "^";
			SetConsoleTextAttribute(handle2, console_screen_buffer_info2.wAttributes);
		}
		else
			cerr << "^";
#else
		cerr << "\033[32m^\033[0m";
#endif
		cerr << "\n\n";	
		if (current_pos < stlist->end())
			return *current_pos;
		else
			return nullptr;
	}

	const int diagnostics_reporter::get_error_count() {
		return error_count;
	}
}
