#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <string>
#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
using namespace std;

// Arquivos
FILE* arq_produtos;
FILE* arq_clientes;
FILE* arq_fornecedor;
FILE* arq_usuario;
FILE* arq_vendas;
FILE* arq_fechamentos;
FILE* arq_hist_cliente;

bool turnoAberto = false;
double valorInicialTurno = 0.0;
double vendasTurno_Dinheiro = 0.0;
double vendasTurno_Cartao = 0.0;
double vendasTurno_Pix = 0.0;
double vendasTurno_Fiado = 0.0;

struct DataHora {
	int dia;
	int mes;
	int ano;
	int hora;
	int minuto;
	int segundo;
};

struct Produtos {
	int id_produto = 0;
	char codigo_barra[30];
	char nome[30];
	char categoria[20];
	double preco_custo = 0.0;
	double preco_venda = 0.0;
	int estoque = 0;
};

struct Fornecedor {
	int id_fornecedor = 0;
	char nome[40];
	char cnpj[20];
	char tipo_produto[30];
	char contato[15];
};

struct Clientes {
	int id_cliente;
	char nome[40];
	char cpf[15];
	char telefone[15];
	double saldo_devedor = 0;
};

struct Usuario {
	int id_usuario = 0;
	enum tipoPerfil { ADMIN, CAIXA } perfil;
	char nome[20];
	char senha[20];
};

struct Vendas {
	int id_venda;
	char nome_Item[40];
	double quant_vendido = 0.0;
	double valor_total = 0.0;
	int formaPagamento;
};

struct FechamentoCaixa {
	int id_fechamento;
	DataHora dataHora;
	double total_vendas;
};

struct HistoricoCliente {
	int id_cliente;
	DataHora data;
	char tipo[20];
	char descricao[50];
	double valor;
};

struct ItemCarrinho {
	Produtos produto;
	int quantidade;
};

struct ProdutoVendido {
	char nome[40];
	int total_vendido = 0;
};

struct Relatorio_Lucro {
	char nome[40];
	int total_vendido = 0;
	double total_receita = 0.0;
	double total_custo = 0.0;
};

// Tratamento de Dados
void ConfirmarSim_Nao(char& confirmar);

// Data e Hora
DataHora ObterDataHoraAtual();

// Menus
void Menu_Primeiro_Uso();
void Menu_Login();
void Menu_Administrador();
void Menu_Caixa();

// Gerenciamento de Produtos
void Gerenciamento_Produto();
int ProximoId_Produto();
bool BuscaProduto(const char* busca, Produtos& ProdEncontrado);
void CadastrarProdutos();
void Listar_Buscar_Produto();
void AtualizarProduto();
void Adicionar_Estoque();
void Excluir_Produto();

// Gerenciamento de Clientes
void MenuGerenciamento_Clientes();
int Proximo_ID_Cliente();
void Cadastrar_Clientes();
bool Buscar_Cliente(const char* busca, Clientes& clienteEncontrado);
void Listar_Buscar_Clientes();
void Atualizar_Cliente();
void Historico_Compras_Cliente();
void Registrar_Pagamento();
void excluir_Cliente();

// Gerenciamento Fornecedores
void Menu_Gerenciamento_Fornecedor();
int Proximo_ID_Fornecedor();
bool Busca_Fornecedor(const char* busca, Fornecedor& fornecedorEncontrado);
void Cadastrar_Fornecedor();
void Listar_Fornecedores();
void Atualizar_Fornecedor();
void Excluir_Fornecedor();

// Gerenciamento Usuário
void Gerenciamento_Usuario();
int Proximo_ID_Usuario();
void Cadastrar_Usuario();
bool Busca_Usuario(const char* busca, Usuario& usuarioEncontrado);
void Listar_Usuarios();
void Atualizar_Usuario();
void Excluir_Usuario();

// Gerenciamento de Vendas
long EncontrarPosProduto(int id_produto_busca);
void Iniciar_Venda();
long EncontrarPosCliente(int id_cliente_busca);
int Proximo_ID_Venda();

// Gerenciamento de Turnos
int Proximo_ID_Fechamento();
void Abrir_Turno();
void Fechar_Turno(const char* nomeOperador);

// Gerenciamento de Relátorios e Análises
void Gerenciamento_Relatorio();
void Relatorio_Fechamento();
DataHora ObterDataHoraAtual();
void Produto_Mais_Vendido();
void Margem_Lucro();
void Clientes_Devedores();
void Estoque_Baixo();

int main(int argc, char** argv) {

	setlocale(LC_ALL, "pt_BR.UTF-8");

	Menu_Primeiro_Uso();

	return 0;
}

// ======================= TRATAMENTO DEDADOS ========================

// Confirmar sim ou não
void ConfirmarSim_Nao(char& confirmar) {
	while (toupper(confirmar) != 'S' && toupper(confirmar) != 'N') {
		system("cls");
		cout << "Dígito Inválido\n\n";
		cout << "Digite Novamente (S/N): ";
		cin >> confirmar;
	}
}


// ================= DATA E HORA ======================
DataHora ObterDataHoraAtual() {
	DataHora agora;
	time_t mytime;
	mytime = time(NULL);
	struct tm tm = *localtime(&mytime);

	agora.dia = tm.tm_mday;
	agora.mes = tm.tm_mon + 1;
	agora.ano = tm.tm_year + 1900;
	agora.hora = tm.tm_hour;
	agora.minuto = tm.tm_min;
	agora.segundo = tm.tm_sec;

	return agora;
}

// ==================== MENUS ==========================

// Menu Primeira vez de uso
void Menu_Primeiro_Uso() {
	char confirmar;
	Usuario usuario;
	int id;

	id = Proximo_ID_Usuario();
	if (id == 1) {
		do {
			system("cls");
			cout << "=================== PDV MERCADO ===================\n\n";
			cout << "Seja Bem-Vindo.\n\nVamos Criar o Primeiro Usuário Como Administrador\n\n\n";
			usuario.id_usuario = id;
			cout << "Digite Nome Usuário: ";
			cin.getline(usuario.nome, sizeof(usuario.nome));
			_strupr(usuario.nome); // ALTERADO: strupr -> _strupr
			cout << "Crie uma Senha: ";
			cin.getline(usuario.senha, sizeof(usuario.senha));

			usuario.perfil = Usuario::ADMIN;

			cout << "\nDeseja Confirmar ? (S/N): ";
			cin >> confirmar;
			cin.ignore();

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') {
				arq_usuario = fopen("Usuario.dat", "ab");
				if (arq_usuario == NULL) {
					return;
				}

				fwrite(&usuario, sizeof(Usuario), 1, arq_usuario);
				fclose(arq_usuario);

				Menu_Login();
			}
		} while (toupper(confirmar) == 'N');
	}
	else {
		Menu_Login();
	}
}

// Menu de Login Admin, Usuário
void Menu_Login() {
	string escolha;
	int opcao;
	do {
		system("cls");
		cout << "================== MENU LOGIN ==================\n\n";
		cout << "1 - Administrador\n";
		cout << "2 - Caixa\n\n";
		cout << "0 - Sair\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1:
			Menu_Administrador();
			break;
		case 2:
			Menu_Caixa();
			break;
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}

// Menu Administrador
void Menu_Administrador() {
	string escolha;
	int opcao;
	char login[30], senha[30];
	Usuario usuarioEncontrado;

	cin.ignore();
	system("cls");
	cout << "============ LOGIN ADMINISTRADOR ============\n\n";
	cout << "Admin: ";
	cin.getline(login, sizeof(login));

	cout << "Senha: ";
	cin.getline(senha, sizeof(senha));

	if (Busca_Usuario(login, usuarioEncontrado)) {
		if (_stricmp(usuarioEncontrado.senha, senha) != 0 || usuarioEncontrado.perfil != Usuario::ADMIN) {
			cout << "\nAdmin ou Senha Inválido\n\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "\nAdmin ou Senha Inválido\n\n";
		system("pause");
		return;
	}

	do {
		system("cls");
		cout << "=========== MENU PRINCIPAL ===========\n\n";
		cout << "1 - Iniciar Nova Venda\n";
		cout << "2 - Gerenciar Produtos e Estoque\n";
		cout << "3 - Gerenciar Clientes\n";
		cout << "4 - Gerenciar Fornecedores\n";
		cout << "5 - Gerenciar Usuários\n";
		cout << "6 - Relatórios e Análises\n\n";
		cout << "0 - Sair\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1:
			Iniciar_Venda();
			break;
		case 2:
			Gerenciamento_Produto();
			break;
		case 3:
			MenuGerenciamento_Clientes();
			break;
		case 4:
			Menu_Gerenciamento_Fornecedor();
			break;
		case 5:
			Gerenciamento_Usuario();
			break;
		case 6:
			Gerenciamento_Relatorio();
			break;
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}

// Menu Usuário
void Menu_Caixa() {
	char login[30], senha[30], confirmar;
	string escolha;
	int opcao;
	Usuario usuarioEncontrado;

	cin.ignore();
	system("cls");
	cout << "============== LOGIN USUÁRIO ==============\n\n";
	cout << "Usuário: ";
	cin.getline(login, sizeof(login));

	cout << "Senha: ";
	cin.getline(senha, sizeof(senha));

	if (Busca_Usuario(login, usuarioEncontrado)) {
		if (_stricmp(usuarioEncontrado.senha, senha) != 0 || usuarioEncontrado.perfil != Usuario::CAIXA) {
			cout << "\nUsuário ou Senha Inválido\n\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "\nAdmin ou Senha Inválido\n\n";
		system("pause");
		return;
	}

	do {
		system("cls");
		cout << "=========== MENU PRINCIPAL ===========\n\n";
		if (turnoAberto) {
			cout << "STATUS: TURNO ABERTO (Caixa Inicial R$ " << fixed << setprecision(2) << valorInicialTurno << " )\n\n";
		}
		else {
			cout << "STATUS: TURNO FECHADO\n\n";
		}

		cout << "1 - Iniciar Venda\n";
		cout << "2 - Consultar Produto\n";
		cout << "3 - Consultar Cliente\n";
		cout << "4 - Abrir Turno\n";
		cout << "5 - Fechar Turno\n\n";
		cout << "0 - Sair\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1: {
			if (turnoAberto) {
				Iniciar_Venda();
			}
			else {
				system("cls");
				cout << "O turno Está Fechado\n\n";
				cout << "Por Favor, Abra o Turno Opção 4, Antes de Iniciar Vendas\n\n";
				system("pause");
			}
			break;
		}
		case 2:
			Listar_Buscar_Produto();
			break;
		case 3:
			Listar_Buscar_Clientes();
			break;
		case 4:
			Abrir_Turno();
			break;
		case 5:
			Fechar_Turno(usuarioEncontrado.nome);
			break;
		case 0: {
			if (turnoAberto) {
				system("cls");
				cout << "ATENÇÃO: O Turno Está Aberto.\n\n";
				cout << "Deseja Fechar Antes de Sair ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
				if (toupper(confirmar) == 'S') {
					Fechar_Turno(usuarioEncontrado.nome);
				}
			}
			break;
		}
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}


// ========================= GERENCIAMENTO DE PRODUTOS ===========================

// Gerenciar Produtos
void Gerenciamento_Produto() {
	string escolha;
	int opcao;

	do {
		system("cls");
		cout << "====== GERENCIAMENTO DE PRODUTOS E ESTOQUE ======\n\n";
		cout << "1 - Cadastrar Novo Produto\n";
		cout << "2 - Listar/Buscar Produtos\n";
		cout << "3 - Atualizar Produtos\n";
		cout << "4 - Registrar Entrada de Estoque\n";
		cout << "5 - Excluir Produto\n\n";
		cout << "0 - Voltar ao Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1:
			CadastrarProdutos();
			break;
		case 2:
			Listar_Buscar_Produto();
			break;
		case 3:
			AtualizarProduto();
			break;
		case 4:
			Adicionar_Estoque();
			break;
		case 5:
			Excluir_Produto();
			break;
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}

// Proximo ID Produto
int ProximoId_Produto() {
	arq_produtos = fopen("Produtos.dat", "rb");
	if (arq_produtos == NULL) {
		return 1;
	}

	fseek(arq_produtos, 0, SEEK_END);

	if (ftell(arq_produtos) == 0) {
		fclose(arq_produtos);
		return 1;
	}

	fseek(arq_produtos, -((long)sizeof(Produtos)), SEEK_END);

	Produtos ultimoProd;
	fread(&ultimoProd, sizeof(Produtos), 1, arq_produtos);
	fclose(arq_produtos);

	return ultimoProd.id_produto + 1;
}

// Cadastrar Produtos
void CadastrarProdutos() {
	char confirmar;
	int id;
	Produtos prod;

	do {
		system("cls");
		cout << "============= CADASTRAR PRODUTOS =============\n\n";
		prod.id_produto = ProximoId_Produto();
		cout << "ID: " << prod.id_produto;

		Produtos prodEncontrado;
		cout << "\nCódigo: ";
		cin.ignore();
		cin.getline(prod.codigo_barra, sizeof(prod.codigo_barra));
		_strupr(prod.codigo_barra);

		if (BuscaProduto(prod.codigo_barra, prodEncontrado)) {
			system("cls");
			cout << "Código Já Cadastrado.\n\nDeseja Tentar Outro Código ?(S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') continue;
			else return;
		}

		cout << "Nome: ";
		cin.getline(prod.nome, sizeof(prod.nome));
		_strupr(prod.nome);

		if (BuscaProduto(prod.nome, prodEncontrado)) {
			system("cls");
			cout << "Produto Já Cadastrado.\n\nDeseja Tentar Outro Produto ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') continue;
			else return;
		}

		cout << "Categoria: ";
		cin.getline(prod.categoria, sizeof(prod.categoria));
		_strupr(prod.categoria);
		cout << "Preço Custo: R$ ";
		cin >> prod.preco_custo;
		cout << "Preço Venda: R$ ";
		cin >> prod.preco_venda;
		cout << "Quantidade Estoque: ";
		cin >> prod.estoque;

		cout << "\n\nDeseja Confirmar Cadastro ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
		if (toupper(confirmar) == 'S') {
			arq_produtos = fopen("Produtos.dat", "ab");
			if (arq_produtos == NULL) {
				cout << "\nErro ao Abrir o arquivo\n\n";
				system("pause");
				return;
			}

			fwrite(&prod, sizeof(prod), 1, arq_produtos);
			fclose(arq_produtos);

			cout << "\n\nProduto Cadastrado Com Sucesso\n\n";
		}
		else {
			cout << "\n\nCadastro Cancelador\n\n";
		}

		cout << "\nDeseja Cadastrar Outro Produto ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Busca Produto
bool BuscaProduto(const char* busca, Produtos& prodEncontrado) {
	arq_produtos = fopen("Produtos.dat", "rb");
	if (arq_produtos == NULL) {
		return false;
	}

	Produtos prod;
	while (fread(&prod, sizeof(prod), 1, arq_produtos) == 1) {
		if (_stricmp(prod.codigo_barra, busca) == 0 || _stricmp(prod.nome, busca) == 0) {
			prodEncontrado = prod;
			fclose(arq_produtos);
			return true;
		}
	}
	fclose(arq_produtos);
	return false;
}

// Listar e Buscar Produto
void Listar_Buscar_Produto() {
	string escolha;
	int opcao;
	char busca[30], confirmar;

	do {
		system("cls");
		cout << "============ LISTAR / BUSCAR PRODUTOS ============\n\n";
		cout << "1 - Listar Todos Produtos\n";
		cout << "2 - Busca Produto\n\n";
		cout << "0 - Voltar ao Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1: {
			system("cls");
			Produtos produto;
			arq_produtos = fopen("Produtos.dat", "rb");
			if (arq_produtos == NULL) {
				cout << "Erro na abertura do arquivo\n\n";
				system("pause");
				return;
			}
			cout << "============================================== LISTA DE PRODUTOS ==============================================\n\n";
			cout << left
				<< setw(8) << "ID"
				<< setw(15) << "CÓDIGO"
				<< setw(20) << "NOME"
				<< setw(20) << "CATEGORIA"
				<< setw(20) << "PREÇO CUSTO"
				<< setw(20) << "PREÇO VENDA"
				<< setw(15) << "ESTOQUE";
			cout << "\n===============================================================================================================\n\n";

			while (fread(&produto, sizeof(Produtos), 1, arq_produtos) == 1) {
				cout << left
					<< setw(8) << produto.id_produto
					<< setw(15) << produto.codigo_barra
					<< setw(20) << produto.nome
					<< setw(20) << produto.categoria
					<< "R$ " << setw(18) << fixed << setprecision(2) << produto.preco_custo
					<< "R$ " << setw(16) << fixed << setprecision(2) << produto.preco_venda
					<< setw(15) << produto.estoque;
				cout << "\n---------------------------------------------------------------------------------------------------------------\n";
			}
			cout << endl;
			system("pause");
			fclose(arq_produtos);

			break;
		}
		case 2: {
			do {
				system("cls");
				cout << "============================================== LISTA DE PRODUTOS ==============================================\n\n";
				cout << "Digito o Código ou Nome do Produto: ";
				cin.ignore();
				cin.getline(busca, sizeof(busca));

				Produtos prodEncontrado;
				if (BuscaProduto(busca, prodEncontrado)) {
					cout << "\n===============================================================================================================\n\n";
					cout << left
						<< setw(8) << "ID"
						<< setw(15) << "CÓDIGO"
						<< setw(20) << "NOME"
						<< setw(20) << "CATEGORIA"
						<< setw(20) << "PREÇO CUSTO"
						<< setw(20) << "PREÇO VENDA"
						<< setw(15) << "ESTOQUE";
					cout << "\n===============================================================================================================\n\n";
					cout << left
						<< setw(8) << prodEncontrado.id_produto
						<< setw(15) << prodEncontrado.codigo_barra
						<< setw(20) << prodEncontrado.nome
						<< setw(20) << prodEncontrado.categoria
						<< "R$ " << setw(18) << fixed << setprecision(2) << prodEncontrado.preco_custo
						<< "R$ " << setw(16) << fixed << setprecision(2) << prodEncontrado.preco_venda
						<< setw(15) << prodEncontrado.estoque;
					cout << "\n---------------------------------------------------------------------------------------------------------------\n";
				}
				else {
					cout << "\nNenhum Produto encontrado";
				}
				cout << "\n\nDeseja Buscar Outro Produto ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
			} while (toupper(confirmar) == 'S');
			break;
		}
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}

// Atualizar Produto
void AtualizarProduto() {
	char busca[30], confirmar;
	string escolha;
	int opcao;
	long pos = -1;
	Produtos produto_leitura;
	Produtos prodEncontrado;

	do {
		system("cls");
		cout << "==================== ATUALIZAR PRODUTO ====================\n\n";
		cout << "Digite Código ou Nome do Produto: ";
		cin.ignore();
		cin.getline(busca, sizeof(busca));

		if (BuscaProduto(busca, prodEncontrado)) {
			pos = -1;
			arq_produtos = fopen("Produtos.dat", "rb");
			if (arq_produtos == NULL) {
				cout << "Erro na abertura do arquivo\n\n";
				system("pause");
				return;
			}

			while (fread(&produto_leitura, sizeof(Produtos), 1, arq_produtos) == 1) {
				if (produto_leitura.id_produto == prodEncontrado.id_produto) {
					pos = ftell(arq_produtos) - sizeof(Produtos);
					break;
				}
			}
			fclose(arq_produtos);

			if (pos == -1) {
				cout << "Erro: Produto encontrado, mas posição no arquivo não localizada.\n";
				cout << "O arquivo pode estar dessincronizado.\n\n";
				system("pause");
				continue;
			}

			Produtos prodEditar = prodEncontrado;
			do {
				system("cls");
				cout << "Produto: " << prodEditar.nome << "\n\n";
				cout << left << setw(20) << "CAMPO" << "VALOR ATUAL\n";
				cout << "---------------------------------------------------------\n\n";

				cout << left << setw(20) << "1 - Nome" << prodEditar.nome << "\n"
					<< setw(20) << "2 - Categoria" << prodEditar.categoria << "\n"
					<< setw(21) << "3 - Preço Custo" << "R$ " << fixed << setprecision(2) << prodEditar.preco_custo << "\n"
					<< setw(21) << "4 - Preço Venda" << "R$ " << fixed << setprecision(2) << prodEditar.preco_venda << "\n\n";

				cout << "---------------------------------------------------------\n";
				cout << "0 - Salvar Alterações e Voltar\n";
				cout << "9 - Cancelar Alterações e Voltar\n\n";
				cout << "Digite o Campo Que Deseja Alterar (ou 0 ou 9): ";
				cin >> escolha;

				if (escolha.size() == 1 && isdigit(escolha[0])) {
					opcao = escolha[0] - '0';
				}
				else {
					opcao = -1;
				}

				switch (opcao) {
				case 1: {
					cout << "\nNovo Nome: ";
					cin.ignore();
					cin.getline(prodEditar.nome, sizeof(prodEditar.nome));
					_strupr(prodEditar.nome);
					break;
				}
				case 2: {
					cout << "\nNova Categoria: ";
					cin.ignore();
					cin.getline(prodEditar.categoria, sizeof(prodEditar.categoria));
					_strupr(prodEditar.categoria);
					break;
				}
				case 3: {
					cout << "\n Novo Preço Custo: R$ ";
					cin >> prodEditar.preco_custo;
					break;
				}
				case 4: {
					cout << "\nNovo Preço Venda: R$ ";
					cin >> prodEditar.preco_venda;
					break;
				}
				case 0: {
					cout << "\nConfirmar e Salvar Todas Alterações ? (S/N): ";
					cin >> confirmar;

					ConfirmarSim_Nao(confirmar);
					if (toupper(confirmar) == 'N') {
						cout << "\nAlteração Cancelada\n\n";
						system("pause");
					}
					else {
						arq_produtos = fopen("Produtos.dat", "r+b");
						if (arq_produtos == NULL) {
							cout << "ERRO FATAL: Não foi possível reabrir o arquivo para salvar!\n\n";
							system("pause");
							return;
						}

						fseek(arq_produtos, pos, SEEK_SET);
						fwrite(&prodEditar, sizeof(Produtos), 1, arq_produtos);

						fclose(arq_produtos);

						cout << "\nProduto atualizado com sucesso!\n\n";
					}
					break;
				}
				case 9: {
					cout << "\nAlteração Cancelada\n\n";
					system("pause");
					return;
					break;
				}
				default: {
					cout << "\nOpção Inválida!\n\n";
					system("pause");
					break;
				}
				}

			} while (opcao != 0 && opcao != 9);

		}
		else {
			cout << "\nProduto Não Encontrado\n";
		}

		cout << "\n\nDeseja Atualizar Outro Produto ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Adicionar Estoque
void Adicionar_Estoque() {
	char confirmar, busca[30];
	long pos = -1;
	Produtos prod_leitura;
	Produtos prodEncontrado;

	do {
		system("cls");
		cout << "================== ADICIONAR ESTOQUE ==================\n\n";
		cout << "Digite Código ou Nome do Produto: ";
		cin.ignore();
		cin.getline(busca, sizeof(busca));
		_strupr(busca);

		if (BuscaProduto(busca, prodEncontrado)) {
			pos = -1;
			arq_produtos = fopen("Produtos.dat", "rb");
			if (arq_produtos == NULL) {
				cout << "Erro na abertura do arquivo\n\n";
				system("pause");
				return;
			}

			while (fread(&prod_leitura, sizeof(Produtos), 1, arq_produtos) == 1) {
				if (prod_leitura.id_produto == prodEncontrado.id_produto) {
					pos = ftell(arq_produtos) - sizeof(Produtos);
					break;
				}
			}
			fclose(arq_produtos);

			if (pos == -1) {
				cout << "Erro: Produto encontrado, mas posição no arquivo não localizada.\n";
				cout << "O arquivo pode estar dessincronizado.\n\n";
				system("pause");
				continue;
			}

			Produtos prodEditar = prodEncontrado;
			system("cls");
			cout << "ID: " << prodEditar.id_produto;
			cout << "\nNome: " << prodEditar.nome;
			cout << "\nEstoque AtuaL: " << prodEditar.estoque;
			cout << "\n------------------------------------------------\n\n";

			int novoEstoque;
			cout << "Digite Quantidade: ";
			cin >> novoEstoque;
			cout << "------------------------------------------------\n\n";

			cout << "Deseja Confirmar ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') {
				prodEditar.estoque += novoEstoque;

				arq_produtos = fopen("Produtos.dat", "r+b");
				if (arq_produtos == NULL) {
					cout << "Erro: Não foi possível reabrir o arquivo para salvar\n\n";
					system("pause");
					return;
				}
				fseek(arq_produtos, pos, SEEK_SET);
				fwrite(&prodEditar, sizeof(Produtos), 1, arq_produtos);
				fclose(arq_produtos);

				cout << "\n\nEstoque Adicionado Com Sucesso\n\n";
			}
			else {
				cout << "\n\nOperação Cancelada\n\n";
			}
		}
		else {
			cout << "\n\nProduto Não encontrado\n\n";
		}

		cout << "Deseja Adicionar Outro Estoque ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Excluir Produto
void Excluir_Produto() {
	char busca[30], confirmar;
	Produtos prod_leitura;
	Produtos prodEncontrado;
	FILE* arq_temp;

	do {
		system("cls");
		cout << "=============================================== EXCLUIR PRODUTO ===============================================\n\n";
		cout << "Digite Código ou Nome do Produto: ";
		cin.ignore();
		cin.getline(busca, sizeof(busca));

		if (BuscaProduto(busca, prodEncontrado)) {
			cout << "\n===============================================================================================================\n\n";
			cout << left
				<< setw(8) << "ID"
				<< setw(15) << "CÓDIGO"
				<< setw(20) << "NOME"
				<< setw(20) << "CATEGORIA"
				<< setw(20) << "PREÇO CUSTO"
				<< setw(20) << "PREÇO VENDA"
				<< setw(15) << "ESTOQUE";
			cout << "\n===============================================================================================================\n\n";
			cout << left
				<< setw(8) << prodEncontrado.id_produto
				<< setw(15) << prodEncontrado.codigo_barra
				<< setw(20) << prodEncontrado.nome
				<< setw(20) << prodEncontrado.categoria
				<< "R$ " << setw(18) << fixed << setprecision(2) << prodEncontrado.preco_custo
				<< "R$ " << setw(16) << fixed << setprecision(2) << prodEncontrado.preco_venda
				<< setw(15) << prodEncontrado.estoque;
			cout << "\n---------------------------------------------------------------------------------------------------------------\n\n";

			if (prodEncontrado.estoque > 0) {
				cout << "Produto Não Pode Ser Excluido. Possui Estoque\n\n";
			}
			else {

				cout << "Deseja Excluir Este Produto ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
				if (toupper(confirmar) == 'S') {
					arq_produtos = fopen("Produtos.dat", "rb");
					if (arq_produtos == NULL) {
						cout << "Erro: não foi possível abrir o arquivo\n\n";
						system("pause");
						return;
					}

					arq_temp = fopen("Temp.dat", "wb");
					if (arq_temp == NULL) {
						cout << "Erro: Não foi possível criar o arquivo temporário\n\n";
						system("pause");
						return;
					}

					while (fread(&prod_leitura, sizeof(Produtos), 1, arq_produtos) == 1) {
						if (prod_leitura.id_produto != prodEncontrado.id_produto) {
							fwrite(&prod_leitura, sizeof(Produtos), 1, arq_temp);
						}
					}
					fclose(arq_produtos);
					fclose(arq_temp);

					if (remove("Produtos.dat") != 0) {
						cout << "Erro: Não foi possível excluir arquivo original\n\n";
						cout << "Os dados estão seguros em Temp.dat\n\n";
						system("pause");
						return;
					}

					if (rename("Temp.dat", "Produtos.dat") != 0) {
						cout << "Erro: Não foi possível renomear o arquivo temporário\n\n";
						cout << "Os dados estão seguros em Tem.dat\n\n";
						system("pause");
						return;
					}

					cout << "\nProduto Excluido Com Sucesso\n\n";
				}
				else {
					cout << "\nExclusão Cancelada\n\n";
				}
			}
		}
		else {
			cout << "\nProduto Não Encontrado\n\n";
		}

		cout << "Deseja Excluir Outro Produto ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}


// ============================== GERENCIAMENTO DE CLIENTES ==============================

// Menu Gerenciamento de clientes
void MenuGerenciamento_Clientes() {
	string escolha;
	int opcao;

	do {
		system("cls");
		cout << "======= GERENCIAMENTO DE CLIENTES =======\n\n";
		cout << "1 - Cadastrar Cliente\n";
		cout << "2 - Listar/Buscar Clientes\n";
		cout << "3 - Atualizar Cliente\n";
		cout << "4 - Histórico de Compras\n";
		cout << "5 - Registrar Pagamento de dívida\n";
		cout << "6 - Excluir Cliente\n\n";
		cout << "0 - Voltar ao Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1:
			Cadastrar_Clientes();
			break;
		case 2:
			Listar_Buscar_Clientes();
			break;
		case 3:
			Atualizar_Cliente();
			break;
		case 4:
			Historico_Compras_Cliente();
			break;
		case 5:
			Registrar_Pagamento();
			break;
		case 6:
			excluir_Cliente();
			break;
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}

	} while (opcao != 0);
}

// Próximo ID Cliente
int Proximo_ID_Cliente() {
	arq_clientes = fopen("Clientes.dat", "rb");
	if (arq_clientes == NULL) {
		return 1;
	}

	fseek(arq_clientes, 0, SEEK_END);
	if (ftell(arq_clientes) == 0) {
		fclose(arq_clientes);
		return 1;
	}

	fseek(arq_clientes, -((long)sizeof(Clientes)), SEEK_END);
	Clientes ultimo_cliente;
	fread(&ultimo_cliente, sizeof(Clientes), 1, arq_clientes);
	fclose(arq_clientes);

	return ultimo_cliente.id_cliente + 1;
}

// Cadastrar Clientes
void Cadastrar_Clientes() {
	char confirmar;
	Clientes cliente;
	Clientes clienteEncontrado;

	do {
		system("cls");
		cout << "=================== CADASTRAR CLIENTE ===================\n\n";
		cliente.id_cliente = Proximo_ID_Cliente();
		cout << "ID: " << cliente.id_cliente;

		cout << "\nNome: ";
		cin.ignore();
		cin.getline(cliente.nome, sizeof(cliente.nome));
		_strupr(cliente.nome);

		if (Buscar_Cliente(cliente.nome, clienteEncontrado)) {
			system("cls");
			cout << "Cliente Já Cadastrado.\n\nDeseja Tentar Outro Cliente ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') continue;
			else return;
		}

		cout << "CPF: ";
		cin.getline(cliente.cpf, sizeof(cliente.cpf));

		if (Buscar_Cliente(cliente.cpf, clienteEncontrado)) {
			system("cls");
			cout << "CPF Já Cadastrado.\n\nDeseja Tentar Outro CPF ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') continue;
			else return;
		}

		cout << "Contato: ";
		cin.getline(cliente.telefone, sizeof(cliente.telefone));

		cout << "Saldo Devedor: ";
		cin >> cliente.saldo_devedor;

		cout << "\nDeseja Confirmar Cadastro ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
		if (toupper(confirmar) == 'S') {
			arq_clientes = fopen("Clientes.dat", "ab");
			if (arq_clientes == NULL) {
				cout << "Erro: Não foi possível abrir arquivo\n\n";
				system("pause");
				return;
			}

			fwrite(&cliente, sizeof(cliente), 1, arq_clientes);
			fclose(arq_clientes);

			cout << "\n\nCliente Cadastrado Com Sucesso\n\n";
		}
		else {
			cout << "\n\nCadastro Cancelado\n\n";
		}
		cout << "Deseja Cadastrar Outro Cliente ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Busca Cliente
bool Buscar_Cliente(const char* busca, Clientes& clienteEncontrado) {
	arq_clientes = fopen("Clientes.dat", "rb");
	if (arq_clientes == NULL) {
		return false;
	}

	Clientes cliente;
	while (fread(&cliente, sizeof(cliente), 1, arq_clientes) == 1) {
		if (_stricmp(cliente.nome, busca) == 0 || _stricmp(cliente.cpf, busca) == 0) {
			clienteEncontrado = cliente;
			fclose(arq_clientes);
			return true;
		}
	}
	fclose(arq_clientes);
	return false;
}

// Listar / Buscar Clientes
void Listar_Buscar_Clientes() {
	char busca[40], confirmar;
	string escolha;
	Clientes cliente;
	int opcao;

	do {
		system("cls");
		cout << "========== LISTAR / BUSCAR CLIENTES ==========\n\n";
		cout << "1 - Listar Todos Clientes\n";
		cout << "2 - Buscar Cliente\n\n";
		cout << "0 - Voltar ao Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1: {
			arq_clientes = fopen("Clientes.dat", "rb");
			if (arq_clientes == NULL) {
				return;
			}
			system("cls");
			cout << "======================================= LISTAR TODOS CLIENTES ========================================\n\n";
			cout << left
				<< setw(14) << "ID"
				<< setw(25) << "NOME"
				<< setw(25) << "CPF"
				<< setw(25) << "CONTATO"
				<< setw(15) << "SALDO DEVEDOR";
			cout << "\n======================================================================================================\n\n";

			while (fread(&cliente, sizeof(Clientes), 1, arq_clientes) == 1) {
				cout << left
					<< setw(14) << cliente.id_cliente
					<< setw(25) << cliente.nome
					<< setw(25) << cliente.cpf
					<< setw(25) << cliente.telefone
					<< "R$ " << setw(15) << fixed << setprecision(2) << cliente.saldo_devedor;
				cout << "\n------------------------------------------------------------------------------------------------------\n";
			}
			fclose(arq_clientes);
			cout << endl;
			system("pause");
			break;
		}
		case 2: {
			do {
				system("cls");
				cout << "======================================= LISTAR TODOS CLIENTES ========================================\n\n";
				cout << "Digite Nome ou CPF do Cliente: ";
				cin.ignore();
				cin.getline(busca, sizeof(busca));

				Clientes clienteEncontrado;
				if (Buscar_Cliente(busca, clienteEncontrado)) {
					cout << "\n======================================================================================================\n\n";
					cout << left
						<< setw(14) << "ID"
						<< setw(25) << "NOME"
						<< setw(25) << "CPF"
						<< setw(25) << "CONTATO"
						<< setw(15) << "SALDO DEVEDOR";
					cout << "\n======================================================================================================\n\n";

					cout << left
						<< setw(14) << clienteEncontrado.id_cliente
						<< setw(25) << clienteEncontrado.nome
						<< setw(25) << clienteEncontrado.cpf
						<< setw(25) << clienteEncontrado.telefone
						<< "R$ " << setw(15) << fixed << setprecision(2) << clienteEncontrado.saldo_devedor;
					cout << "\n------------------------------------------------------------------------------------------------------\n\n";
				}
				else {
					cout << "Cliente Não Encontrado\n\n";
				}

				cout << "Deseja Consultar Outro Cliente ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
			} while (toupper(confirmar) == 'S');
			break;
		}
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito inválido. Digite novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}

// Atualização de Dados do Cliente
void Atualizar_Cliente() {
	char busca[40], confirmar;
	string escolha;
	int opcao;
	long pos = -1;
	Clientes cliente_leitura;
	Clientes clienteEncontrado;

	do {
		system("cls");
		cout << "================= ATUALIZAÇÃO DE CLIENTES =================\n\n";
		cout << "Digite o CPF ou Nome do Cliente: ";
		cin.ignore();
		cin.getline(busca, sizeof(busca));

		if (Buscar_Cliente(busca, clienteEncontrado)) {
			arq_clientes = fopen("Clientes.dat", "rb");
			if (arq_clientes == NULL) {
				return;
			}

			while (fread(&cliente_leitura, sizeof(Clientes), 1, arq_clientes) == 1) {
				if (cliente_leitura.id_cliente == clienteEncontrado.id_cliente) {
					pos = ftell(arq_clientes) - sizeof(Clientes);
					break;
				}
			}
			fclose(arq_clientes);

			if (pos == -1) {
				cout << "Erro: Cliente encontrado, mas posição no arquivo não localizada.\n";
				system("pause");
				continue;
			}

			Clientes clienteEditar = clienteEncontrado;
			do {
				system("cls");
				cout << "Cliente: " << clienteEditar.nome << "\n\n";
				cout << left << setw(25) << "CAMPO" << "DADOS";
				cout << "\n-----------------------------------------------\n\n";

				cout << left << setw(25) << "1 - Nome:" << clienteEditar.nome << endl
					<< setw(25) << "2 - CPF:" << clienteEditar.cpf << endl
					<< setw(25) << "3 - Contato" << clienteEditar.telefone << "\n";
				cout << "-----------------------------------------------\n\n";
				cout << "0 - Salvar Alterações e voltar\n";
				cout << "9 - Cancelar Alterações e Voltar\n\n";
				cout << "Digite o Campo Que Deseja Alterar (ou 0 ou 9): ";
				cin >> escolha;

				if (escolha.size() == 1 && isdigit(escolha[0])) {
					opcao = escolha[0] - '0';
				}
				else {
					opcao = -1;
				}

				switch (opcao) {
				case 1: {
					cout << "\nNovo Nome: ";
					cin.ignore();
					cin.getline(clienteEditar.nome, sizeof(clienteEditar.nome));
					_strupr(clienteEditar.nome);
					break;
				}
				case 2: {
					cout << "\nNovo CPF: ";
					cin.ignore();
					cin.getline(clienteEditar.cpf, sizeof(clienteEditar.cpf));
					break;
				}
				case 3: {
					cout << "\nContato: ";
					cin.ignore();
					cin.getline(clienteEditar.telefone, sizeof(clienteEditar.telefone));
					break;
				}
				case 0: {
					cout << "\n\nDeseja Salvar todas Alteraões ? (S/N): ";
					cin >> confirmar;

					ConfirmarSim_Nao(confirmar);
					if (toupper(confirmar) == 'N') {
						cout << "\n\nAlterações Canceladas\n\n";
					}
					else {
						arq_clientes = fopen("Clientes.dat", "r+b");
						if (arq_clientes == NULL) {
							return;
						}

						fseek(arq_clientes, pos, SEEK_SET);
						fwrite(&clienteEditar, sizeof(Clientes), 1, arq_clientes);

						fclose(arq_clientes);

						cout << "\n\nCliente Atualizado Com Sucesso\n\n";
					}
					break;
				}
				case 9:
					cout << "\n\nAlteração Cancelada\n\n";
					system("pause");
					return;
					break;
				default:
					system("cls");
					cout << "Dígito Inválido. Digite Novamente\n\n";
					system("pause");
					break;
				}
			} while (opcao != 0 && opcao != 9);
		}
		else {
			cout << "Cliente NãoEncontrado\n\n";
		}

		cout << "Deseja Atualizar Outro Cliente ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Histórico de Compras do Cliente
void Historico_Compras_Cliente() {
	char busca[40];
	Clientes clienteEncontrado;
	HistoricoCliente item_leitura;

	system("cls");
	cout << "================= HISTÓRICO DO CLIENTE =================\n\n";
	cout << "Digite o CPF ou Nome do Cliente: ";
	cin.ignore();
	cin.getline(busca, sizeof(busca));
	_strupr(busca);

	if (Buscar_Cliente(busca, clienteEncontrado)) {

		system("cls");
		cout << "Cliente: " << clienteEncontrado.nome << " (ID: " << clienteEncontrado.id_cliente << ")\n";
		cout << "Telefone: " << clienteEncontrado.telefone << "\n\n";
		cout << "SALDO DEVEDOR ATUAL: R$ " << fixed << setprecision(2) << clienteEncontrado.saldo_devedor << "\n";
		cout << "=================================================================================\n\n";

		cout << left
			<< setw(22) << "DATA/HORA"
			<< setw(18) << "TIPO"
			<< setw(25) << "DESCRIÇÃO"
			<< "VALOR (R$)";
		cout << "\n=================================================================================\n";

		arq_hist_cliente = fopen("HistCliente.dat", "rb");

		if (arq_hist_cliente == NULL) {
			cout << "\nNenhum histórico de transações fiado/pagamento encontrado.\n";
		}
		else {
			bool encontrouRegistros = false;

			while (fread(&item_leitura, sizeof(HistoricoCliente), 1, arq_hist_cliente) == 1) {
				if (item_leitura.id_cliente == clienteEncontrado.id_cliente) {
					encontrouRegistros = true;

					cout << left << setfill('0')
						<< setw(2) << item_leitura.data.dia << "/"
						<< setw(2) << item_leitura.data.mes << "/"
						<< setw(4) << item_leitura.data.ano << " "
						<< setw(2) << item_leitura.data.hora << ":"
						<< setw(2) << item_leitura.data.minuto
						<< setfill(' ');

					cout << "      "
						<< setw(18) << item_leitura.tipo
						<< setw(25) << item_leitura.descricao;
					cout << fixed << setprecision(2) << item_leitura.valor;
					cout << "\n---------------------------------------------------------------------------------\n";
				}
			}
			fclose(arq_hist_cliente);

			if (!encontrouRegistros) {
				cout << "\nNenhum histórico de transações fiado/pagamento encontrado para este cliente.\n";
			}
		}
		cout << "\n=================================================================================\n";
		cout << "Saldo Final: R$ " << fixed << setprecision(2) << clienteEncontrado.saldo_devedor << "\n\n";
	}
	else {
		cout << "\nCliente não encontrado.\n\n";
	}
	system("pause");
}

// Registrar Pagamento
void Registrar_Pagamento() {
	char busca[40], confirmar;
	long pos = -1;
	Clientes cliente_leitura;
	Clientes clienteEncontrado;

	do {
		system("cls");
		cout << "================= REGISTRAR PAGAMENTO =================\n\n";
		cout << "Digite o CPF ou Nome do Cliente: ";
		cin.ignore();
		cin.getline(busca, sizeof(busca));

		if (Buscar_Cliente(busca, clienteEncontrado)) {
			arq_clientes = fopen("Clientes.dat", "rb");
			if (arq_clientes == NULL) {
				return;
			}

			while (fread(&cliente_leitura, sizeof(Clientes), 1, arq_clientes) == 1) {
				if (cliente_leitura.id_cliente == clienteEncontrado.id_cliente) {
					pos = ftell(arq_clientes) - sizeof(Clientes);
					break;
				}
			}
			fclose(arq_clientes);

			Clientes clienteEditar = clienteEncontrado;
			system("cls");
			cout << left << setw(25) << "CLIENTE"
				<< setw(15) << "SALDO DEVEDOR";
			cout << "\n======================================\n\n";

			cout << left << setw(25) << clienteEditar.nome
				<< "R$ " << setw(15) << fixed << setprecision(2) << clienteEditar.saldo_devedor;
			cout << "\n--------------------------------------\n\n";

			double pagamento;
			cout << "Valor Pagamento: R$ ";
			cin >> pagamento;

			while (pagamento < 0 || round(pagamento * 100.0) > round(clienteEditar.saldo_devedor * 100.0)) {
				system("cls");
				cout << "================= REGISTRAR PAGAMENTO =================\n\n";
				cout << "Erro, valor inválido, negativo ou maior que o saldo devedor!\n\n";
				cout << "Saldo Devedor Atual: R$ " << fixed << setprecision(2) << clienteEditar.saldo_devedor << endl;
				cout << "\nDigite o Valor do Pagamento: R$ ";
				cin >> pagamento;
			}

			cout << "\n\nDeseja Confirmar ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') {
				long long saldoDevedorCentavos = round(clienteEditar.saldo_devedor * 100.0);
				long long valorPagamentoCentavos = round(pagamento * 100.0);

				saldoDevedorCentavos -= valorPagamentoCentavos;

				clienteEditar.saldo_devedor = saldoDevedorCentavos / 100.0;

				arq_clientes = fopen("Clientes.dat", "r+b");
				if (arq_clientes == NULL) {
					return;
				}
				fseek(arq_clientes, pos, SEEK_SET);
				fwrite(&clienteEditar, sizeof(Clientes), 1, arq_clientes);
				fclose(arq_clientes);

				HistoricoCliente hist;
				hist.id_cliente = clienteEncontrado.id_cliente;
				hist.data = ObterDataHoraAtual();
				strcpy(hist.tipo, "PAGAMENTO");
				strcpy(hist.descricao, "Pagamento de Divida");
				hist.valor = -pagamento; // Valor negativo, pois é um pagamento

				arq_hist_cliente = fopen("HistCliente.dat", "ab");
				// *** CORREÇÃO APLICADA AQUI ***
				if (arq_hist_cliente != NULL) { // Correto: != NULL
					fwrite(&hist, sizeof(HistoricoCliente), 1, arq_hist_cliente);
					fclose(arq_hist_cliente);
				}
				else {
					cout << "\nERRO: Nao foi possivel salvar no historico do cliente.\n";
				}


				cout << "\nPagamento Realizado Com Sucesso\n\n";
			}
			else {
				cout << "\nPagamento Cancelado\n\n";
			}
		}
		else {
			cout << "\nCliente Não Encontrado\n\n";
		}

		cout << "Deseja Realizar Outro Pagamento ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Excluir Cliente
void excluir_Cliente() {
	char busca[40], confirmar;
	Clientes clienteEncontrado;
	Clientes cliente_leitura;
	FILE* arqTemp;

	do {
		system("cls");
		cout << "=========================================== EXCLUIR CLIENTE ===========================================\n\n";
		cout << "Digite o CPF ou Nome do Cliente: ";
		cin.ignore();
		cin.getline(busca, sizeof(busca));

		if (Buscar_Cliente(busca, clienteEncontrado)) {
			cout << "\n======================================================================================================\n\n";
			cout << left
				<< setw(14) << "ID"
				<< setw(25) << "NOME"
				<< setw(25) << "CPF"
				<< setw(25) << "CONTATO"
				<< setw(15) << "SALDO DEVEDOR";
			cout << "\n======================================================================================================\n\n";

			cout << left
				<< setw(14) << clienteEncontrado.id_cliente
				<< setw(25) << clienteEncontrado.nome
				<< setw(25) << clienteEncontrado.cpf
				<< setw(25) << clienteEncontrado.telefone
				<< "R$ " << setw(15) << fixed << setprecision(2) << clienteEncontrado.saldo_devedor;
			cout << "\n------------------------------------------------------------------------------------------------------\n\n";

			if (clienteEncontrado.saldo_devedor > 0) {
				cout << "Cliente Não Pode Ser Excluido. Possui Saldo Ativo\n\n";
			}
			else {
				cout << "Deseja Excluir ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
				if (toupper(confirmar) == 'S') {
					arq_clientes = fopen("Clientes.dat", "rb");
					if (arq_clientes == NULL) {
						return;
					}

					arqTemp = fopen("Temp.dat", "wb");
					if (arqTemp == NULL) {
						return;
					}
					while (fread(&cliente_leitura, sizeof(Clientes), 1, arq_clientes) == 1) {
						if (cliente_leitura.id_cliente != clienteEncontrado.id_cliente) {
							fwrite(&cliente_leitura, sizeof(Clientes), 1, arqTemp);
						}
					}
					fclose(arq_clientes);
					fclose(arqTemp);

					if (remove("Clientes.dat") != 0) {
						return;
					}

					if (rename("Temp.dat", "Clientes.dat") != 0) {
						return;
					}

					cout << "\nCliente Excluido Com Sucesso\n\n";
				}
				else {
					cout << "\n\nExclusão Cancelado\n\n";
				}
			}
		}
		else {
			cout << "\nCliente Não Encontrado\n\n";
		}

		cout << "Deseja excluir Outro Cliente ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar); // *** CORRIGIDO: Faltava (confirmar) ***
	} while (toupper(confirmar) == 'S');
}


// ===================== GERENCIAMENTO DE FONECEDORES =========================

// Menu Gerenciamento de Forncedor
void Menu_Gerenciamento_Fornecedor() {
	string escolha;
	int opcao;

	do {
		system("cls");
		cout << "========== GERENCIAMENTO DE FORNECEDORES ==========\n\n";
		cout << "1 - Cadastrar Novo Fornecedor\n";
		cout << "2 - Listar Todos os Fornecedores\n";
		cout << "3 - Atualizar Fornecedor\n";
		cout << "4 - Excluir Fornecedor\n\n";
		cout << "0 - Voltar ao Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1:
			Cadastrar_Fornecedor();
			break;
		case 2:
			Listar_Fornecedores();
			break;
		case 3:
			Atualizar_Fornecedor();
			break;
		case 4:
			Excluir_Fornecedor();
			break;
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}

	} while (opcao != 0);
}

// Próximo ID fornecedor
int Proximo_ID_Fornecedor() {
	arq_fornecedor = fopen("Fornecedor.dat", "rb");
	if (arq_fornecedor == NULL) {
		return 1;
	}

	fseek(arq_fornecedor, 0, SEEK_END);
	if (ftell(arq_fornecedor) == 0) {
		fclose(arq_fornecedor);
		return 1;
	}

	fseek(arq_fornecedor, -((long)sizeof(Fornecedor)), SEEK_END);
	Fornecedor ultimo_fornecedor;
	fread(&ultimo_fornecedor, sizeof(Fornecedor), 1, arq_fornecedor);
	fclose(arq_fornecedor);

	return ultimo_fornecedor.id_fornecedor + 1;
}

// Cadastrar Fornecedor
void Cadastrar_Fornecedor() {
	char confirmar;
	Fornecedor fornecedor;
	Fornecedor fornecedorEncontrado;

	do {
		system("cls");
		cout << "================ CADASTRAR FORNECEDOR ================\n\n";
		fornecedor.id_fornecedor = Proximo_ID_Fornecedor();
		cout << "ID: " << fornecedor.id_fornecedor;

		cout << "\nNome: ";
		cin.ignore();
		cin.getline(fornecedor.nome, sizeof(fornecedor.nome));
		_strupr(fornecedor.nome);

		if (Busca_Fornecedor(fornecedor.nome, fornecedorEncontrado)) {
			system("cls");
			cout << "Fornecedor Já Cadastrador.\n\nDeseja Tentar Outro Fornecedor ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') continue;
			else return;
		}

		cout << "CNPJ: ";
		cin.getline(fornecedor.cnpj, sizeof(fornecedor.cnpj));

		if (Busca_Fornecedor(fornecedor.cnpj, fornecedorEncontrado)) {
			system("cls");
			cout << "CNPJ Já Cadastrado.\n\nDeseja Tentar Outro CNPJ ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') continue;
			else return;
		}

		cout << "Tipo Produto: ";
		cin.getline(fornecedor.tipo_produto, sizeof(fornecedor.tipo_produto));
		_strupr(fornecedor.tipo_produto);

		cout << "Contato: ";
		cin.getline(fornecedor.contato, sizeof(fornecedor.contato));

		cout << "\n\nDeseja Confirmar o Cadastro ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
		if (toupper(confirmar) == 'S') {
			arq_fornecedor = fopen("Fornecedor.dat", "ab");
			if (arq_fornecedor == NULL) {
				return;
			}

			fwrite(&fornecedor, sizeof(fornecedor), 1, arq_fornecedor);
			fclose(arq_fornecedor);

			cout << "\n\nCadastro Realizador Com Sucesso\n\n";
		}
		else {
			cout << "\n\nCadastro Cancelado\n\n";
		}

		cout << "Deseja Cadastrar Outro Fornecedor ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Busca Fornecedores
bool Busca_Fornecedor(const char* busca, Fornecedor& fornecedorEncontrado) {
	arq_fornecedor = fopen("Fornecedor.dat", "rb");
	if (arq_fornecedor == NULL) {
		return false;
	}

	Fornecedor fornecedor;
	while (fread(&fornecedor, sizeof(fornecedor), 1, arq_fornecedor) == 1) {
		if (_stricmp(fornecedor.nome, busca) == 0 || _stricmp(fornecedor.cnpj, busca) == 0) {
			fornecedorEncontrado = fornecedor;
			fclose(arq_fornecedor);
			return true;
		}
	}
	fclose(arq_fornecedor);
	return false;
}

// Listar Fonecedores
void Listar_Fornecedores() {
	Fornecedor fornecedor;

	system("cls");
	arq_fornecedor = fopen("Fornecedor.dat", "rb");
	if (arq_fornecedor == NULL) {
		return;
	}

	cout << "========================================== LISTA DE FORNECEDORES ==========================================\n\n";
	cout << left << setw(14) << "ID"
		<< setw(30) << "NOME"
		<< setw(28) << "CNPJ"
		<< setw(23) << "TIPO"
		<< setw(23) << "CONTATO";
	cout << "\n===========================================================================================================\n\n";

	while (fread(&fornecedor, sizeof(Fornecedor), 1, arq_fornecedor) == 1) {
		cout << left << setw(14) << fornecedor.id_fornecedor
			<< setw(30) << fornecedor.nome
			<< setw(28) << fornecedor.cnpj
			<< setw(23) << fornecedor.tipo_produto
			<< setw(23) << fornecedor.contato;
		cout << "\n-----------------------------------------------------------------------------------------------------------\n";
	}
	fclose(arq_fornecedor);

	cout << endl;
	system("pause");
}

// Atualizar Dados Fornecedor
void Atualizar_Fornecedor() {
	char confirmar, busca[30];
	int opcao;
	long pos = -1;
	string escolha;
	Fornecedor fornecedor_leitura;
	Fornecedor fornecedorEncontrado;

	do {
		system("cls");
		cout << "================ ATUALIZAÇÃO DE FORNCEDORES ================\n\n";
		cout << "Digite CNPJ ou Nome do Fornecedor: ";
		cin.ignore();
		cin.getline(busca, sizeof(busca));

		if (Busca_Fornecedor(busca, fornecedorEncontrado)) {
			arq_fornecedor = fopen("Fornecedor.dat", "rb");
			if (arq_fornecedor == NULL) {
				return;
			}

			while (fread(&fornecedor_leitura, sizeof(Fornecedor), 1, arq_fornecedor) == 1) {
				if (fornecedor_leitura.id_fornecedor == fornecedorEncontrado.id_fornecedor) {
					pos = ftell(arq_fornecedor) - sizeof(Fornecedor);
					break;
				}
			}
			fclose(arq_fornecedor);

			if (pos == -1) {
				cout << "Erro\n\n";
				system("pause");
				return;
			}

			Fornecedor fornecedorEditar = fornecedorEncontrado;
			do {
				system("cls");
				cout << "Fornecedor: " << fornecedorEditar.nome;
				cout << "\n--------------------------------------------\n\n";

				cout << "1 - Nome: " << fornecedorEditar.nome;
				cout << "\n2 - CNPJ: " << fornecedorEditar.cnpj;
				cout << "\n3 - Tipo: " << fornecedorEditar.tipo_produto;
				cout << "\n4 - Contato: " << fornecedorEditar.contato;
				cout << "\n--------------------------------------------\n\n";
				cout << "0 - Salvar Alterações\n";
				cout << "9 - Cancelar e Voltar\n\n";
				cout << "Digite Opção: ";
				cin >> escolha;

				if (escolha.size() == 1 && isdigit(escolha[0])) {
					opcao = escolha[0] - '0';
				}
				else {
					opcao = -1;
				}

				switch (opcao) {
				case 1: {
					cout << "\nNovo Nome: ";
					cin.ignore();
					cin.getline(fornecedorEditar.nome, sizeof(fornecedorEditar.nome));
					_strupr(fornecedorEditar.nome);
					break;
				}
				case 2: {
					cout << "\nNovo CNPJ: ";
					cin.ignore();
					cin.getline(fornecedorEditar.cnpj, sizeof(fornecedorEditar.cnpj));
					break;
				}
				case 3: {
					cout << "\nNovo Tipo: ";
					cin.ignore();
					cin.getline(fornecedorEditar.tipo_produto, sizeof(fornecedorEditar.tipo_produto));
					_strupr(fornecedorEditar.tipo_produto);
					break;
				}
				case 4: {
					cout << "\nNovo Contato: ";
					cin.ignore();
					cin.getline(fornecedorEditar.contato, sizeof(fornecedorEditar.contato));
					break;
				}
				case 0: {
					cout << "\n\nDeseja Salvar Alterações ? (S/N): ";
					cin >> confirmar;

					ConfirmarSim_Nao(confirmar);
					if (toupper(confirmar) == 'S') {
						arq_fornecedor = fopen("Fornecedor.dat", "r+b");
						if (arq_fornecedor == NULL) {
							return;
						}

						fseek(arq_fornecedor, pos, SEEK_SET);
						fwrite(&fornecedorEditar, sizeof(Fornecedor), 1, arq_fornecedor);
						fclose(arq_fornecedor);

						cout << "\n\nAlteração Confirmado Com Sucesso\n\n";
					}
					else {
						cout << "\n\nAlteração Cancelado\n\n";
					}
					break;
				}
				case 9:
					cout << "\n\nAlteração Cancelado\n\n";
					system("pause");
					return;
					break;
				default:
					system("cls");
					cout << "Dígito Inválido. Digite Novamente\n\n";
					system("pause");
					break;
				}
			} while (opcao != 0 && opcao != 9);
		}
		else {
			cout << "\nFornecedr Não Encontrado\n\n";
		}

		cout << "Deseja Alterar Outro Fornecedor ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Excluir Fornecedor
void Excluir_Fornecedor() {
	char confirmar, busca[30];
	Fornecedor fornecedor_leitura;
	Fornecedor fornecedorEncontrado;
	FILE* arqTemp;

	do {
		system("cls");
		cout << "=================== EXCLUIR FORNECEDOR ===================\n\n";
		cout << "Digite CNPJ ou Nome do Fornecedor: ";
		cin.ignore();
		cin.getline(busca, sizeof(busca));

		if (Busca_Fornecedor(busca, fornecedorEncontrado)) {
			system("cls");
			cout << "========================================== LISTA DE FORNECEDORES ==========================================\n\n";
			cout << left << setw(14) << "ID"
				<< setw(30) << "NOME"
				<< setw(28) << "CNPJ"
				<< setw(23) << "TIPO"
				<< setw(23) << "CONTATO";
			cout << "\n===========================================================================================================\n\n";

			cout << left << setw(14) << fornecedorEncontrado.id_fornecedor
				<< setw(30) << fornecedorEncontrado.nome
				<< setw(28) << fornecedorEncontrado.cnpj
				<< setw(23) << fornecedorEncontrado.tipo_produto
				<< setw(23) << fornecedorEncontrado.contato;
			cout << "\n-----------------------------------------------------------------------------------------------------------\n";

			cout << "\nDeseja Excluir ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') {
				arq_fornecedor = fopen("Fornecedor.dat", "rb");
				if (arq_fornecedor == NULL) {
					return;
				}

				arqTemp = fopen("Temp.dat", "wb");
				if (arqTemp == NULL) {
					return;
				}

				while (fread(&fornecedor_leitura, sizeof(Fornecedor), 1, arq_fornecedor) == 1) {
					if (fornecedor_leitura.id_fornecedor != fornecedorEncontrado.id_fornecedor) {
						fwrite(&fornecedor_leitura, sizeof(Fornecedor), 1, arqTemp);
					}
				}
				fclose(arq_fornecedor);
				fclose(arqTemp);

				if (remove("Fornecedor.dat") != 0) {
					return;
				}

				if (rename("Temp.dat", "Fornecedor.dat") != 0) {
					return;
				}

				cout << "\n\nFornecedor Excluido Com Sucesso\n\n";
			}
			else {
				cout << "\n\nExclusão Cancelador\n\n";
			}
		}
		else {
			cout << "\n\nFornecedor Não Encontrado\n\n";
		}

		cout << "Deseja Excluir Outro Fornecedor ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}


// ==================== GERENCIAMENTO DE USUÁRIO ==========================

// Menu Gerenciamento Usuário
void Gerenciamento_Usuario() {
	string escolha;
	int opcao;

	do {
		system("cls");
		cout << "=========== GERENCIAMENTO DE USUÁRIO ===========\n\n";
		cout << "1 - Cadastrar Usuário\n";
		cout << "2 - Listar Usuário\n";
		cout << "3 - Atualizar Usuário\n";
		cout << "4 - Excluir Usuário\n\n";
		cout << "0 - Voltar ao Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1:
			Cadastrar_Usuario();
			break;
		case 2:
			Listar_Usuarios();
			break;
		case 3:
			Atualizar_Usuario();
			break;
		case 4:
			Excluir_Usuario();
			break;
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}

// Próximo ID Usuário
int Proximo_ID_Usuario() {
	arq_usuario = fopen("Usuario.dat", "rb");
	if (arq_usuario == NULL) {
		return 1;
	}

	fseek(arq_usuario, 0, SEEK_END);
	if (ftell(arq_usuario) == 0) {
		fclose(arq_usuario);
		return 1;
	}

	fseek(arq_usuario, -((long)sizeof(Usuario)), SEEK_END);
	Usuario ultimo_usuario;
	fread(&ultimo_usuario, sizeof(Usuario), 1, arq_usuario);
	fclose(arq_usuario);

	return ultimo_usuario.id_usuario + 1;
}

// Cadastrar Usuário
void Cadastrar_Usuario() {
	string escolha;
	char confirmar;
	int opcao;
	Usuario usuario;
	Usuario usuarioEncontrado;

	do {
		usuario.id_usuario = Proximo_ID_Usuario();
		system("cls");
		cout << "================ CADASTRAR USUÁRIO ================\n\n";
		cout << "1 - Administrador\n";
		cout << "2 - Caixa\n\n";
		cout << "0 - Voltar ao Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1: {
			do {
				system("cls");
				cout << "========== CADASTRAR ADMINISTRADOR ==========\n\n";
				cout << "ID: " << usuario.id_usuario << endl;
				cout << "Nome Admin: ";
				cin.ignore();
				cin.getline(usuario.nome, sizeof(usuario.nome));
				_strupr(usuario.nome);

				if (Busca_Usuario(usuario.nome, usuarioEncontrado)) {
					system("cls");
					cout << "Usuário Já Cadstrado.\n\nDeseja Tentar Outro Usuário ? (S/N): ";
					cin >> confirmar;

					ConfirmarSim_Nao(confirmar);
					if (toupper(confirmar) == 'S') continue;
					else return;
				}

				cout << "Senha: ";
				cin.getline(usuario.senha, sizeof(usuario.senha));

				usuario.perfil = Usuario::ADMIN;

				cout << "\n\nDeseja Confirmar Cadastro ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
				if (toupper(confirmar) == 'S') {
					arq_usuario = fopen("Usuario.dat", "ab");
					if (arq_usuario == NULL) {
						return;
					}

					fwrite(&usuario, sizeof(usuario), 1, arq_usuario);
					fclose(arq_usuario);

					cout << "\n\nCadastro Realizado Com Sucesso\n\n";
				}
				else {
					cout << "\n\nCadastro Cancelador\n\n";
				}

				cout << "Deseja Fazer Outro Cadastro ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
			} while (toupper(confirmar) == 'S');
			break;
		}
		case 2: {
			do {
				system("cls");
				cout << "============== CADASTRAR CAIXA ==============\n\n";
				cout << "ID: " << usuario.id_usuario << endl;
				cout << "Nome Usuário: ";
				cin.ignore();
				cin.getline(usuario.nome, sizeof(usuario.nome));
				_strupr(usuario.nome); // ALTERADO: strupr -> _strupr

				if (Busca_Usuario(usuario.nome, usuarioEncontrado)) {
					system("cls");
					cout << "Usuário Já Cadstrado.\n\nDeseja Tentar Outro Usuário ? (S/N): ";
					cin >> confirmar;

					ConfirmarSim_Nao(confirmar);
					if (toupper(confirmar) == 'S') continue;
					else return;
				}

				cout << "Senha: ";
				cin.getline(usuario.senha, sizeof(usuario.senha));

				usuario.perfil = Usuario::CAIXA;

				cout << "\n\nDeseja Confirmar Cadastro ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
				if (toupper(confirmar) == 'S') {
					arq_usuario = fopen("Usuario.dat", "ab");
					if (arq_usuario == NULL) {
						return;
					}

					fwrite(&usuario, sizeof(usuario), 1, arq_usuario);
					fclose(arq_usuario);

					cout << "\n\nCadastro Realizado Com Sucesso\n\n";
				}
				else {
					cout << "\n\nCadastro Cancelador\n\n";
				}

				cout << "Deseja Fazer Outro Cadastro ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
			} while (toupper(confirmar) == 'S');
			break;
		}
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}

// Busca usuário
bool Busca_Usuario(const char* busca, Usuario& usuarioEncontrado) {
	arq_usuario = fopen("Usuario.dat", "rb");
	if (arq_usuario == NULL) {
		return false;
	}
	Usuario usuario;
	while (fread(&usuario, sizeof(Usuario), 1, arq_usuario) == 1) {
		if (_stricmp(usuario.nome, busca) == 0) {
			usuarioEncontrado = usuario;
			fclose(arq_usuario);
			return true;
		}
	}
	fclose(arq_usuario);
	return false;
}

// Listar usuários
void Listar_Usuarios() {
	Usuario usuario;

	arq_usuario = fopen("Usuario.dat", "rb");
	if (arq_usuario == NULL) {
		return;
	}

	system("cls");
	cout << "================== LISTA DE USUÁRIOS ==================\n\n";
	cout << "0 - Administrador\n";
	cout << "1 - Caixa\n\n";
	cout << "=======================================================\n";
	cout << left << setw(10) << "ID"
		<< setw(20) << "USUÁRIO"
		<< setw(20) << "SENHA"
		<< setw(20) << "TIPO";
	cout << "\n=======================================================\n\n";

	while (fread(&usuario, sizeof(Usuario), 1, arq_usuario) == 1) {
		cout << left << setw(10) << usuario.id_usuario
			<< setw(20) << usuario.nome
			<< setw(20) << usuario.senha
			<< setw(20) << usuario.perfil;
		cout << "\n-------------------------------------------------------\n";
	}
	fclose(arq_usuario);

	cout << endl;
	system("pause");
}

// Atualizar Usuário
void Atualizar_Usuario() {
	string escolha;
	int opcao;
	long pos = -1;
	char busca[30], confirmar;
	Usuario usuarioEncontrado;
	Usuario usuario_leitura;

	do {
		cin.ignore();
		system("cls");
		cout << "=========== ATUALIZAR DADOS USUÁRIO ===========\n\n";
		cout << "Digite Nome do Usuário: ";
		cin.getline(busca, sizeof(busca));

		if (Busca_Usuario(busca, usuarioEncontrado)) {
			arq_usuario = fopen("Usuario.dat", "rb");
			if (arq_usuario == NULL) {
				return;
			}

			while (fread(&usuario_leitura, sizeof(Usuario), 1, arq_usuario) == 1) {
				if (usuario_leitura.id_usuario == usuarioEncontrado.id_usuario) {
					pos = ftell(arq_usuario) - sizeof(Usuario);
					break;
				}
			}
			fclose(arq_usuario);

			if (pos == -1) {
				cout << "Erro\n\n";
				system("pause");
				return;
			}

			Usuario usuarioEditar = usuarioEncontrado;
			do {
				system("cls");
				cout << "Usuário: " << usuarioEditar.nome;
				cout << "\n===============================\n\n";

				cout << "1 - Nome: " << usuarioEditar.nome;
				cout << "\n2 - Senha: " << usuarioEditar.senha;
				cout << "\n-------------------------------\n\n";
				cout << "0 - Salvar \n";
				cout << "9 - Cancelar Alteração\n\n";
				cout << "Digite Opção: ";
				cin >> escolha;

				if (escolha.size() == 1 && isdigit(escolha[0])) {
					opcao = escolha[0] - '0';
				}
				else {
					opcao = -1;
				}

				switch (opcao) {
				case 1: {
					cout << "\nNovo Nome: ";
					cin.ignore();
					cin.getline(usuarioEditar.nome, sizeof(usuarioEditar.nome));
					_strupr(usuarioEditar.nome);
					break;
				}
				case 2: {
					cout << "\nNova Senha: ";
					cin.ignore();
					cin.getline(usuarioEditar.senha, sizeof(usuarioEditar.senha));
					break;
				}
				case 0: {
					cout << "\nDeseja Confirmar Alteração ? (S/N): ";
					cin >> confirmar;

					ConfirmarSim_Nao(confirmar);
					if (toupper(confirmar) == 'S') {
						arq_usuario = fopen("Usuario.dat", "r+b");
						if (arq_usuario == NULL) {
							return;
						}

						fseek(arq_usuario, pos, SEEK_SET);
						fwrite(&usuarioEditar, sizeof(Usuario), 1, arq_usuario);
						fclose(arq_usuario);

						cout << "\n\nAlteração Realizado Com Sucesso\n\n";
					}
					else {
						cout << "\n\nAlteração Cancelado\n\n";
					}
					break;
				}
				case 9:
					cout << "\nAlteração Cancelado\n\n";
					system("pause");
					return;
					break;
				default:
					system("cls");
					cout << "Dígito Inválido. Digite Novamente\n\n";
					system("pause");
					break;
				}
			} while (opcao != 0 && opcao != 9);
		}

		cout << "Deseja Alterar Outro Usuário ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}

// Excluir Usuário
void Excluir_Usuario() {
	char busca[30], confirmar;
	Usuario usuario_leitura;
	Usuario usuarioEncontrado;
	FILE* arqTemp;

	do {
		cin.ignore();
		system("cls");
		cout << "=================== EXCLUIR USUÁRIO ===================\n\n";
		cout << "Digite Nome do Usuário: ";
		cin.getline(busca, sizeof(busca));

		if (Busca_Usuario(busca, usuarioEncontrado)) {
			cout << "\n=======================================================\n";
			cout << left << setw(10) << "ID"
				<< setw(20) << "USUÁRIO"
				<< setw(20) << "SENHA"
				<< setw(20) << "TIPO";
			cout << "\n=======================================================\n\n";

			cout << left << setw(10) << usuarioEncontrado.id_usuario
				<< setw(20) << usuarioEncontrado.nome
				<< setw(20) << usuarioEncontrado.senha
				<< setw(20) << usuarioEncontrado.perfil;
			cout << "\n-------------------------------------------------------\n\n";

			cout << "Deseja Excluir ? (S/N): ";
			cin >> confirmar;

			ConfirmarSim_Nao(confirmar);
			if (toupper(confirmar) == 'S') {
				arq_usuario = fopen("Usuario.dat", "rb");
				if (arq_usuario == NULL) {
					return;
				}

				arqTemp = fopen("Temp.dat", "wb");
				if (arqTemp == NULL) {
					return;
				}

				while (fread(&usuario_leitura, sizeof(Usuario), 1, arq_usuario) == 1) {
					if (usuario_leitura.id_usuario != usuarioEncontrado.id_usuario) {
						fwrite(&usuario_leitura, sizeof(Usuario), 1, arqTemp);
					}
				}
				fclose(arq_usuario);
				fclose(arqTemp);

				if (remove("Usuario.dat") != 0) {
					return;
				}

				if (rename("Temp.dat", "Usuario.dat") != 0) {
					return;
				}

				cout << "\n\nExclusão Realizado Com Sucesso\n\n";
			}
			else {
				cout << "\n\nExclusão Cancelado\n\n";
			}
		}
		else {
			cout << "\nUsuário Não Encontrado\n\n";
		}

		cout << "Deseja Excluir Outro Usuário ? (S/N): ";
		cin >> confirmar;

		ConfirmarSim_Nao(confirmar);
	} while (toupper(confirmar) == 'S');
}


// ===================== GERENCIAMENTO VENDA =========================

// posição do produto
long EncontrarPosProduto(int id_produto_busca) {
	FILE* arq_temp_prod = fopen("Produtos.dat", "rb");
	if (arq_temp_prod == NULL) {
		return -1;
	}

	Produtos prod_leitura;
	long pos = -1;

	while (fread(&prod_leitura, sizeof(Produtos), 1, arq_temp_prod) == 1) {
		if (prod_leitura.id_produto == id_produto_busca) {
			pos = ftell(arq_temp_prod) - sizeof(Produtos);
			break;
		}
	}

	fclose(arq_temp_prod);
	return pos;
}

// posição do cliente
long EncontrarPosCliente(int id_cliente_busca) {
	FILE* arq_temp_cli = fopen("Clientes.dat", "rb");
	if (arq_temp_cli == NULL) {
		return -1;
	}

	Clientes cli_leitura;
	long pos = -1;

	while (fread(&cli_leitura, sizeof(Clientes), 1, arq_temp_cli) == 1) {
		if (cli_leitura.id_cliente == id_cliente_busca) {
			pos = ftell(arq_temp_cli) - sizeof(Clientes);
			break;
		}
	}

	fclose(arq_temp_cli);
	return pos;
}

// Proximo ID Venda
int Proximo_ID_Venda() {
	arq_vendas = fopen("Vendas.dat", "rb");
	if (arq_vendas == NULL) {
		return 1;
	}

	fseek(arq_vendas, 0, SEEK_END);

	if (ftell(arq_vendas) == 0) {
		fclose(arq_vendas);
		return 1;
	}

	Vendas venda_leitura;
	int id = 0;

	fseek(arq_vendas, 0, SEEK_SET);
	while (fread(&venda_leitura, sizeof(Vendas), 1, arq_vendas) == 1) {
		if (venda_leitura.id_venda > id) {
			id = venda_leitura.id_venda;
		}
	}
	fclose(arq_vendas);

	return id + 1;
}

// Iniciar Venda
void Iniciar_Venda() {
	string escolha;
	int opcao;
	char busca[30], confirmar;
	double total = 0.0;

	ItemCarrinho carrinho[50];
	int numItens = 0;

	do {
		system("cls");
		cout << "=================================== NOVA VENDA ===================================\n\n";

		if (numItens > 0) {
			cout << "Itens no Carrinho:\n";
			cout << "----------------------------------------------------------------------------------\n";
			for (int i = 0; i < numItens; i++) {
				cout << left
					<< setw(3) << i + 1
					<< setw(25) << carrinho[i].produto.nome
					<< " Qtd: " << setw(5) << carrinho[i].quantidade
					<< " Vl. Unit: R$ " << fixed << setprecision(2) << setw(8) << carrinho[i].produto.preco_venda
					<< " Vl. Total: R$ " << fixed << setprecision(2) << setw(8) << (carrinho[i].produto.preco_venda * carrinho[i].quantidade)
					<< endl;
			}
			cout << "----------------------------------------------------------------------------------\n";
			cout << "TOTAL DA VENDA: R$ " << fixed << setprecision(2) << total << endl;
			cout << "==================================================================================\n\n";
		}
		else {
			cout << "Carrinho Vazio\n\n";
		}

		cout << "1 - Adicionar Produto ao Carrinho\n";
		cout << "2 - Finalizar Venda\n\n";
		cout << "0 - Cancelar Venda e Voltar Para o Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1: {
			do {
				system("cls");
				cout << "=============================== ADICIONAR PRODUTO ===============================\n\n";
				cout << "Digite o Código ou Nome do Produto (ou '0' para Voltar): ";
				cin.ignore();
				cin.getline(busca, sizeof(busca));
				_strupr(busca);
				cout << "==================================================================================\n\n";

				if (strcmp(busca, "0") == 0) {
					break;
				}

				Produtos prodEncontrado;
				if (BuscaProduto(busca, prodEncontrado)) {

					cout << "Produto: " << prodEncontrado.nome;
					cout << "\nValor: R$ " << fixed << setprecision(2) << prodEncontrado.preco_venda;
					cout << "\nEstoque Atual: " << prodEncontrado.estoque;

					int quantidade;
					cout << "\n\nDigite Quantidade: ";
					cin >> quantidade;

					if (quantidade <= 0) {
						cout << "\nQuantidade inválida!\n\n";
					}
					else if (quantidade > prodEncontrado.estoque) {
						cout << "\nEstoque insuficiente! (Disponível: " << prodEncontrado.estoque << ")\n\n";
					}
					else {
						carrinho[numItens].produto = prodEncontrado;
						carrinho[numItens].quantidade = quantidade;
						numItens++;

						total += (prodEncontrado.preco_venda * quantidade);

						cout << "\nItem adicionado ao carrinho!\n\n";
					}
				}
				else {
					cout << "Produto Não Encontrado\n\n";
				}

				cout << "Deseja Inserir Outro Produto ? (S/N): ";
				cin >> confirmar;

				ConfirmarSim_Nao(confirmar);
			} while (toupper(confirmar) == 'S');
			break;
		}

		case 2: {
			if (numItens == 0) {
				cout << "\nCarrinho vazio! Adicione produtos antes de finalizar.\n\n";
				system("pause");
				break;
			}

			system("cls");
			cout << "================================ FINALIZAR VENDA ===============================\n\n";
			cout << "TOTAL DA COMPRA: R$ " << fixed << setprecision(2) << total << endl;
			cout << "\n-------------------------------------------------------------------------------\n";

			string escolhaPag;
			int formaPagamento;
			bool pagamentoOK = false;

			int idDaVendaAtual = Proximo_ID_Venda();

			cout << "Forma de Pagamento:\n";
			cout << "1 - Dinheiro\n";
			cout << "2 - Cartão de Crédito/Débito\n";
			cout << "3 - PIX\n";
			cout << "4 - Fiado (Anotar no Cliente)\n\n";
			cout << "0 - Voltar ao Carrinho\n\n";
			cout << "Digite Opção: ";
			cin >> escolhaPag;

			if (escolhaPag.size() == 1 && isdigit(escolhaPag[0])) {
				formaPagamento = escolhaPag[0] - '0';
			}
			else {
				formaPagamento = -1;
			}

			system("cls");
			switch (formaPagamento) {
			case 1: {
				double valorPago;
				cout << "\nValor Recebido (R$): ";
				cin >> valorPago;

				if (valorPago < total) {
					cout << "\nValor insuficiente. Pagamento cancelado.\n\n";
					system("pause");
				}
				else {
					double troco = valorPago - total;
					cout << "Troco: R$ " << fixed << setprecision(2) << troco << endl;
					pagamentoOK = true;
				}
				break;
			}
			case 2:
			case 3:
				cout << "\nPagamento Aprovado.\n";
				pagamentoOK = true;
				break;

			case 4: {
				char buscaCliente[40];
				Clientes clienteEncontrado;
				cout << "\nDigite o CPF ou Nome do Cliente: ";
				cin.ignore();
				cin.getline(buscaCliente, sizeof(buscaCliente));
				_strupr(buscaCliente);

				if (Buscar_Cliente(buscaCliente, clienteEncontrado)) {
					long posCliente = EncontrarPosCliente(clienteEncontrado.id_cliente);

					if (posCliente == -1) {
						cout << "ERRO\n";
						system("pause");
						break;
					}

					cout << "\nCliente: " << clienteEncontrado.nome;
					cout << "\nSaldo Devedor Atual: R$ " << fixed << setprecision(2) << clienteEncontrado.saldo_devedor;
					cout << "\nNovo Saldo será: R$ " << fixed << setprecision(2) << (clienteEncontrado.saldo_devedor + total);
					cout << "\n\nConfirmar Venda Fiado? (S/N): ";
					cin >> confirmar;

					ConfirmarSim_Nao(confirmar);
					if (toupper(confirmar) == 'S') {
						clienteEncontrado.saldo_devedor += total;

						arq_clientes = fopen("Clientes.dat", "r+b");
						if (arq_clientes == NULL) {
							break;
						}
						fseek(arq_clientes, posCliente, SEEK_SET);
						fwrite(&clienteEncontrado, sizeof(Clientes), 1, arq_clientes);
						fclose(arq_clientes);

						cout << "\nSaldo do cliente atualizado com sucesso.\n";
						pagamentoOK = true;

						HistoricoCliente hist;
						hist.id_cliente = clienteEncontrado.id_cliente;
						hist.data = ObterDataHoraAtual();
						strcpy(hist.tipo, "COMPRA FIADO");
						hist.valor = total; // Valor positivo, aumenta a dívida

						sprintf(hist.descricao, "Venda ID %d", idDaVendaAtual);

						arq_hist_cliente = fopen("HistCliente.dat", "ab");
						// *** CORREÇÃO APLICADA AQUI ***
						if (arq_hist_cliente != NULL) { // Correto: != NULL
							fwrite(&hist, sizeof(HistoricoCliente), 1, arq_hist_cliente);
							fclose(arq_hist_cliente);
						}
						else {
							cout << "\nERRO: Nao foi possivel salvar no historico do cliente.\n";
						}
					}
					else {
						cout << "\nVenda Fiado Cancelada.\n";
					}

				}
				else {
					cout << "\nCliente não encontrado. Venda Fiado não pode ser concluída.\n";
				}
				break;
			}
			case 0:
				cout << "\nVoltando ao carrinho...\n";
				break;
			default:
				cout << "\nOpção de pagamento inválida!\n";
				break;
			}

			if (pagamentoOK) {
				cout << "\nProcessando Venda... Atualizando Estoque e Registrando...\n";

				switch (formaPagamento) {
				case 1:
					vendasTurno_Dinheiro += total;
					break;
				case 2:
					vendasTurno_Cartao += total;
					break;
				case 3:
					vendasTurno_Pix += total;
					break;
				case 4:
					vendasTurno_Fiado += total;
					break;
				}

				arq_vendas = fopen("Vendas.dat", "ab");
				if (arq_vendas == NULL) {
					return;
				}

				for (int i = 0; i < numItens; i++) {

					long posProduto = EncontrarPosProduto(carrinho[i].produto.id_produto);

					if (posProduto != -1) {
						Produtos prodAtualizar;
						arq_produtos = fopen("Produtos.dat", "r+b");

						if (arq_produtos != NULL) {
							fseek(arq_produtos, posProduto, SEEK_SET);
							fread(&prodAtualizar, sizeof(Produtos), 1, arq_produtos);

							prodAtualizar.estoque -= carrinho[i].quantidade;

							fseek(arq_produtos, posProduto, SEEK_SET);
							fwrite(&prodAtualizar, sizeof(Produtos), 1, arq_produtos);

							fclose(arq_produtos);
						}
						else {
							cout << "ERRO: Nao abriu Produtos.dat para item " << carrinho[i].produto.nome << "\n";
						}

					}
					else {
						cout << "ERRO FATAL: Posicao do produto " << carrinho[i].produto.nome << " nao encontrada.\n";
						cout << "VENDA COBRADA, MAS ESTOQUE NAO ATUALIZADO.\n";
					}

					if (arq_vendas != NULL) {
						Vendas itemVendido;
						itemVendido.id_venda = idDaVendaAtual;
						strcpy(itemVendido.nome_Item, carrinho[i].produto.nome);
						itemVendido.quant_vendido = carrinho[i].quantidade;
						itemVendido.valor_total = carrinho[i].produto.preco_venda * carrinho[i].quantidade;
						itemVendido.formaPagamento = formaPagamento;

						fwrite(&itemVendido, sizeof(Vendas), 1, arq_vendas);
					}
				}

				if (arq_vendas != NULL) {
					fclose(arq_vendas);
				}

				cout << "\n==================================================================";
				cout << "\nVENDA FINALIZADA COM SUCESSO!\n";
				cout << "==================================================================\n\n";

				total = 0.0;
				numItens = 0;

				system("pause");

				opcao = 0;

			}
			else {
				if (formaPagamento != 0) {
					system("pause");
				}
			}

			break;
		}

		case 0: {
			if (numItens > 0) {
				cout << "\nVocê tem itens no carrinho. Deseja realmente cancelar a venda? (S/N): ";
				cin >> confirmar;
				ConfirmarSim_Nao(confirmar);

				if (toupper(confirmar) == 'N') {
					opcao = -1;
					continue;
				}
			}

			cout << "\nVenda Cancelada. Voltando ao Menu Principal...\n";
			break;
		}
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}


// ===================== TURNOS CAIXA ======================

// Próximo ID fechamento
int Proximo_ID_Fechamento() {
	arq_fechamentos = fopen("Fechamentos.dat", "rb");
	if (arq_fechamentos == NULL) {
		return 1;
	}

	fseek(arq_fechamentos, 0, SEEK_END);
	if (ftell(arq_fechamentos) == 0) {
		fclose(arq_fechamentos);
		return 1;
	}

	fseek(arq_fechamentos, -((long)sizeof(FechamentoCaixa)), SEEK_END);
	FechamentoCaixa ultimo;
	fread(&ultimo, sizeof(FechamentoCaixa), 1, arq_fechamentos);
	fclose(arq_fechamentos);

	return ultimo.id_fechamento + 1;
}

// Abrir Caixa
void Abrir_Turno() {
	char confirmar;

	if (turnoAberto) {
		system("cls");
		cout << "=================== ABRIR TURNO ===================\n\n";
		cout << "O Turno Já Está Aberto\n\n";
		cout << "Valor Inicial Registrado: R$ " << fixed << setprecision(2) << valorInicialTurno << "\n\n";
		system("pause");
		return;
	}

	system("cls");
	cout << "=================== ABRIR TURNO ===================\n\n";
	cout << "Digite o Valor Inicial: R$ ";
	cin >> valorInicialTurno;

	while (valorInicialTurno < 0) {
		system("cls");
		cout << "Valor Não Pode Ser Negativo.\n\n";
		cout << "Digite Novamente: R$ ";
		cin >> valorInicialTurno;
	}

	cout << "\nConfirmar Abertura do Turno com R$ " << fixed << setprecision(2) << valorInicialTurno << " ? (S/N): ";
	cin >> confirmar;
	cin.ignore();

	ConfirmarSim_Nao(confirmar);
	if (toupper(confirmar) == 'S') {
		turnoAberto = true;

		vendasTurno_Dinheiro = 0.0;
		vendasTurno_Cartao = 0.0;
		vendasTurno_Pix = 0.0;
		vendasTurno_Fiado = 0.0;

		cout << "\nTurno Aberto Com Sucesso\n\n";
	}
	else {
		valorInicialTurno = 0.0;
		cout << "\nAbertura de Turno Cancelado\n\n";
	}
	system("pause");
}

// Fechar Turno
void Fechar_Turno(const char* nomeOperador) {
	char confirmar;

	if (!turnoAberto) {
		system("cls");
		cout << "=================== FECHAR TURNO ===================\n\n";
		cout << "Nenhum Turno Está Aberto\n\n";
		system("pause");
		return;
	}

	system("cls");
	cout << "=================== RELATÓRIO DE FECHAMENTO ===================\n\n";
	cout << "Operador: " << nomeOperador << endl;

	double totalVendas = vendasTurno_Dinheiro + vendasTurno_Cartao + vendasTurno_Pix + vendasTurno_Fiado;
	double valorEsperado = valorInicialTurno + vendasTurno_Dinheiro;

	cout << "-------------------------------------------------------------\n";
	cout << left << setw(30) << "Valor Incial (Troco): " << "R$ " << fixed << setprecision(2) << valorInicialTurno << endl;
	cout << "-------------------------------------------------------------\n\n";
	cout << "VENDAS DO TURNO:\n";

	cout << left << setw(30) << "  + Total em Dinheiro: " << "R$ " << fixed << setprecision(2) << vendasTurno_Dinheiro << endl
		<< setw(31) << "  + Total em Cartão: " << "R$ " << fixed << setprecision(2) << vendasTurno_Cartao << endl
		<< setw(30) << "  + Total em PIX: " << "R$ " << fixed << setprecision(2) << vendasTurno_Pix << endl
		<< setw(30) << "  + Total em Fiado: " << "R$ " << fixed << setprecision(2) << vendasTurno_Fiado << endl;
	cout << "-------------------------------------------------------------\n";
	cout << left << setw(30) << "TOTAL DE VENDAS: " << "R$ " << fixed << setprecision(2) << totalVendas << endl;
	cout << "=============================================================\n";
	cout << left << setw(30) << "VALOR ESPERADO NA GAVETA: " << "R$ " << fixed << setprecision(2) << valorEsperado << endl;
	cout << "(Valor Inicial + Vendas em Dinheiro)\n\n";

	cout << "Deseja Confirmar e Fechar o Turno ? (S/N): ";
	cin >> confirmar;

	ConfirmarSim_Nao(confirmar);
	if (toupper(confirmar) == 'S') {
		FechamentoCaixa fechamento;

		fechamento.id_fechamento = Proximo_ID_Fechamento();

		fechamento.dataHora = ObterDataHoraAtual();

		fechamento.total_vendas = totalVendas;

		arq_fechamentos = fopen("Fechamentos.dat", "ab");
		if (arq_fechamentos == NULL) {
			return;
		}

		fwrite(&fechamento, sizeof(FechamentoCaixa), 1, arq_fechamentos);
		fclose(arq_fechamentos);

		turnoAberto = false;
		valorInicialTurno = 0.0;
		vendasTurno_Dinheiro = 0.0;
		vendasTurno_Cartao = 0.0;
		vendasTurno_Pix = 0.0;
		vendasTurno_Fiado = 0.0;

		cout << "\nTurno Fechado Com Sucesso\n\n";
	}
	else {
		cout << "\nFechamento de Turno Cancelado\n\n";
	}
	system("pause");
}


// =============== GERENCIAMENTO RELATÓRIO E ANÁLISE =====================

// Menu de Relatórios
void Gerenciamento_Relatorio() {
	string escolha;
	int opcao;

	do {
		system("cls");
		cout << "========= RELATÓRIOS E ANÁLISES =========\n\n";
		cout << "1 - Histórico de Fechamento\n";
		cout << "2 - Produtos Mais Vendidos\n";
		cout << "3 - Margem de Lucro\n";
		cout << "4 - Clientes Devedores\n";
		cout << "5 - Estoque Baixo\n\n";
		cout << "0 - Voltar ao Menu Principal\n\n";
		cout << "Digite Opção: ";
		cin >> escolha;

		if (escolha.size() == 1 && isdigit(escolha[0])) {
			opcao = escolha[0] - '0';
		}
		else {
			opcao = -1;
		}

		switch (opcao) {
		case 1:
			Relatorio_Fechamento();
			break;
		case 2:
			Produto_Mais_Vendido();
			break;
		case 3:
			Margem_Lucro();
			break;
		case 4:
			Clientes_Devedores();
			break;
		case 5:
			Estoque_Baixo();
			break;
		case 0:
			break;
		default:
			system("cls");
			cout << "Dígito Inválido. Digite Novamente\n\n";
			system("pause");
			break;
		}
	} while (opcao != 0);
}

// Relatório de fechamento
void Relatorio_Fechamento() {
	FechamentoCaixa fechamento;

	system("cls");
	cout << "================= HISTÓRICO DE FECHAMENTOS DE CAIXA =================\n\n";
	cout << left << setw(10) << "ID"
		<< setw(30) << "DATA/HORA"
		<< "TOTAL DE VENDAS";
	cout << "\n=====================================================================\n";

	arq_fechamentos = fopen("Fechamentos.dat", "rb");
	if (arq_fechamentos == NULL) {
		cout << "Nenhum Dado Registrado\n\n";
		system("pause");
		return;
	}

	while (fread(&fechamento, sizeof(FechamentoCaixa), 1, arq_fechamentos) == 1) {
		cout << left
			<< setw(10) << fechamento.id_fechamento;
		cout << setfill('0')
			<< setw(2) << fechamento.dataHora.dia << "/"
			<< setw(2) << fechamento.dataHora.mes << "/"
			<< setw(4) << fechamento.dataHora.ano << " "
			<< setw(2) << fechamento.dataHora.hora << ":"
			<< setw(2) << fechamento.dataHora.minuto << ":"
			<< setw(2) << fechamento.dataHora.segundo
			<< setfill(' ');
		cout << setw(11) << "";
		cout << "R$ " << fixed << setprecision(2) << fechamento.total_vendas;
		cout << "\n---------------------------------------------------------------------\n";
	}
	fclose(arq_fechamentos);

	cout << endl;
	system("pause");
}

// Produtos Mais Vendidos
void Produto_Mais_Vendido() {
	const int max_produto = 1000;
	ProdutoVendido agregado[max_produto];
	int numUnicos = 0;
	Vendas venda_leitura;

	arq_vendas = fopen("Vendas.dat", "rb");
	if (arq_vendas == NULL) {
		cout << "Nenhum Dao de Vendas Registrado\n\n";
		system("pause");
		return;
	}

	while (fread(&venda_leitura, sizeof(Vendas), 1, arq_vendas) == 1) {
		bool encontrado = false;
		for (int i = 0; i < numUnicos; i++) {
			if (_stricmp(agregado[i].nome, venda_leitura.nome_Item) == 0) {
				agregado[i].total_vendido += (int)venda_leitura.quant_vendido;
				encontrado = true;
				break;
			}
		}
		if (!encontrado) {
			if (numUnicos < max_produto) {
				strcpy(agregado[numUnicos].nome, venda_leitura.nome_Item);
				agregado[numUnicos].total_vendido = (int)venda_leitura.quant_vendido;
				numUnicos++;
			}
		}
	}
	fclose(arq_vendas);

	if (numUnicos == 0) {
		system("cls");
		cout << "Nenhum Produto Foi Vendido Ainda\n\n";
		system("pause");
		return;
	}

	for (int i = 0; i < numUnicos - 1; i++) {
		for (int j = 0; j < numUnicos - i - 1; j++) {
			if (agregado[j].total_vendido < agregado[j + 1].total_vendido) {
				ProdutoVendido temp = agregado[j];
				agregado[j] = agregado[j + 1];
				agregado[j + 1] = temp;
			}
		}
	}

	system("cls");
	cout << "================ RELATÓRIO DE PRODUTOS MAIS VENDIDOS ================\n\n";
	cout << left << setw(10) << "RANK"
		<< setw(40) << "PRODUTO"
		<< "QUANTIDADE VENDIDA";
	cout << "\n=====================================================================\n\n";

	for (int i = 0; i < numUnicos; i++) {
		cout << left << setw(10) << (i + 1)
			<< setw(40) << agregado[i].nome
			<< agregado[i].total_vendido;
		cout << "\n---------------------------------------------------------------------\n";
	}
	cout << endl;
	system("pause");
}

// Relatório de Margem de Lucro
void Margem_Lucro() {
	const int max_produto = 1000;
	Relatorio_Lucro agregado[max_produto];
	int numUnicos = 0;
	Vendas venda_leitura;
	Produtos prodInfo;

	arq_vendas = fopen("Vendas.dat", "rb");
	if (arq_vendas == NULL) {
		cout << "Nenhum Dado de Venda Registrado\n\n";
		system("pause");
		return;
	}

	while (fread(&venda_leitura, sizeof(Vendas), 1, arq_vendas) == 1) {
		double custo_item_atual = 0.0;

		if (BuscaProduto(venda_leitura.nome_Item, prodInfo)) {
			custo_item_atual = prodInfo.preco_custo * venda_leitura.quant_vendido;
		}
		else {
			custo_item_atual = 0.0;
		}

		bool encontrado = false;
		for (int i = 0; i < numUnicos; i++) {
			if (_stricmp(agregado[i].nome, venda_leitura.nome_Item) == 0) {
				agregado[i].total_vendido += (int)venda_leitura.quant_vendido;
				agregado[i].total_receita += venda_leitura.valor_total;
				agregado[i].total_custo += custo_item_atual;
				encontrado = true;
				break;
			}
		}
		if (!encontrado && numUnicos < max_produto) {
			strcpy(agregado[numUnicos].nome, venda_leitura.nome_Item);
			agregado[numUnicos].total_vendido = (int)venda_leitura.quant_vendido;
			agregado[numUnicos].total_receita = venda_leitura.valor_total;
			agregado[numUnicos].total_custo = custo_item_atual;
			numUnicos++;
		}
	}
	fclose(arq_vendas);

	if (numUnicos == 0) {
		system("cls");
		cout << "Nenhum Produto Foi Vendido Ainda\n\n";
		system("pause");
		return;
	}

	system("cls");
	cout << "====================================== RELATÓRIO DE MARGEM DE LUCRO ======================================\n\n";
	cout << left
		<< setw(30) << "PRODUTO"
		<< setw(15) << "QTD VENDIDA"
		<< setw(17) << "TOTAL CUSTO"
		<< setw(17) << "TOTAL RECEITA"
		<< setw(17) << "LUCRO BRUTO"
		<< setw(10) << "MARGEM %";
	cout << "\n==========================================================================================================\n\n";

	double gran_total_custo = 0.0;
	double gran_total_receita = 0.0;
	double gran_total_lucro = 0.0;

	for (int i = 0; i < numUnicos; i++) {
		double lucro_bruto = agregado[i].total_receita - agregado[i].total_custo;
		double margem = (agregado[i].total_receita > 0) ? (lucro_bruto / agregado[i].total_receita) * 100.0 : 0.0;

		gran_total_custo += agregado[i].total_custo;
		gran_total_receita += agregado[i].total_receita;
		gran_total_lucro += lucro_bruto;

		cout << left
			<< setw(30) << agregado[i].nome
			<< setw(15) << agregado[i].total_vendido
			<< "R$ " << setw(14) << fixed << setprecision(2) << agregado[i].total_custo
			<< "R$ " << setw(14) << fixed << setprecision(2) << agregado[i].total_receita
			<< "R$ " << setw(14) << fixed << setprecision(2) << lucro_bruto
			<< setw(7) << fixed << setprecision(2) << margem << "%";
		cout << "\n----------------------------------------------------------------------------------------------------------\n";
	}
	double margem_total = (gran_total_receita > 0) ? (gran_total_lucro / gran_total_receita) * 100.0 : 0.0;

	cout << "\n================================================= TOTAIS =================================================\n\n";
	cout << left
		<< setw(30) << "TOTAL GERAL"
		<< setw(15) << ""
		<< "R$ " << setw(14) << fixed << setprecision(2) << gran_total_custo
		<< "R$ " << setw(14) << fixed << setprecision(2) << gran_total_receita
		<< "R$ " << setw(14) << fixed << setprecision(2) << gran_total_lucro
		<< setw(7) << fixed << setprecision(2) << margem_total << "%";

	cout << "\n==========================================================================================================\n\n";
	system("pause");
}

// Relatório de Clientes Devedores
void Clientes_Devedores() {
	Clientes clientes;

	arq_clientes = fopen("Clientes.dat", "rb");
	if (arq_clientes == NULL) {
		cout << "Nenhum Dado Registrado\n\n";
		return;
	}

	system("cls");
	cout << "========================================= CLIENTES DEVEDORES ==========================================\n\n";
	cout << left
		<< setw(14) << "ID"
		<< setw(25) << "NOME"
		<< setw(25) << "CPF"
		<< setw(25) << "CONTATO"
		<< setw(15) << "SALDO DEVEDOR";
	cout << "\n=======================================================================================================\n\n";

	while (fread(&clientes, sizeof(Clientes), 1, arq_clientes) == 1) {
		if (clientes.saldo_devedor > 0) {
			cout << left
				<< setw(14) << clientes.id_cliente
				<< setw(25) << clientes.nome
				<< setw(25) << clientes.cpf
				<< setw(25) << clientes.telefone
				<< "R$ " << setw(15) << fixed << setprecision(2) << clientes.saldo_devedor;
			cout << "\n-------------------------------------------------------------------------------------------------------\n";
		}
	}
	fclose(arq_clientes);

	cout << endl;
	system("pause");
}

// Relatório de Estoque Baixo
void Estoque_Baixo() {
	Produtos produtos;

	arq_produtos = fopen("Produtos.dat", "rb");
	if (arq_produtos == NULL) {
		cout << "Nenhum Dado Resgistrado\n\n";
		system("pause");
		return;
	}

	system("cls");
	cout << "============================================= RELATÓRIO DE ESTOQUE ============================================\n\n";
	cout << left
		<< setw(8) << "ID"
		<< setw(15) << "CÓDIGO"
		<< setw(20) << "NOME"
		<< setw(20) << "CATEGORIA"
		<< setw(20) << "PREÇO CUSTO"
		<< setw(20) << "PREÇO VENDA"
		<< setw(15) << "ESTOQUE";
	cout << "\n===============================================================================================================\n\n";

	while (fread(&produtos, sizeof(Produtos), 1, arq_produtos) == 1) {
		if (produtos.estoque <= 24) {
			cout << left
				<< setw(8) << produtos.id_produto
				<< setw(15) << produtos.codigo_barra
				<< setw(20) << produtos.nome
				<< setw(20) << produtos.categoria
				<< "R$ " << setw(18) << fixed << setprecision(2) << produtos.preco_custo
				<< "R$ " << setw(16) << fixed << setprecision(2) << produtos.preco_venda
				<< setw(15) << produtos.estoque;
			cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		}
	}
	fclose(arq_produtos);

	cout << endl;
	system("pause");
}