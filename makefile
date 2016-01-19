# Pour compiler en mode test, ajouté la variable MODE avec la valeur 'test' lors
# d'un appel a make :
#
# make run MODE=test
#
# Il peut y avoir des problèmes si le projet est recompilé dans un autre mode.
# Pensez à faire une make clean avant.

OUTPUT=build
FILE_SYSTEM=file_system.img

MODE=normal

.PHONY: clean run kernel doc tools user common

$(OUTPUT).iso: kernel grub/grub.cfg $(OUTPUT)/boot/grub
	cp grub/grub.cfg $(OUTPUT)/boot/grub/
	cp kernel/kernel.elf $(OUTPUT)/boot/
	grub-mkrescue -o $@ $(OUTPUT)

kernel:
	@make -C kernel MODE=$(MODE)

$(OUTPUT)/boot/grub:
	mkdir -p $@

$(FILE_SYSTEM): tools user
	cp user/shell shell
	cp user/shell2 shell2
	cp user/tictactoe tictactoe
	cp user/tictactoejeu.txt tictactoejeu.txt
	cp user/tictactoeacueil.txt tictactoeacueil.txt
	tools/pfscreate $@ 2048 256 4096
	tools/pfsadd $@ shell
	tools/pfsadd $@ image.txt
	tools/pfsadd $@ tictactoe
	tools/pfsadd $@ tictactoeacueil.txt
	tools/pfsadd $@ tictactoejeu.txt
	rm shell
	rm shell2
	rm tictactoe
	rm tictactoejeu.txt
	rm tictactoeacueil.txt

doc:
	@make -C doc

tools:
	@make -C tools

user:
	@make -C user

common:
	@make -C common

run: $(OUTPUT).iso $(FILE_SYSTEM)
	qemu-system-i386 -cdrom $< -hda $(FILE_SYSTEM)

clean:
	@make -C kernel clean
	@make -C doc clean
	@make -C tools clean
	@make -C user clean
	@make -C common clean
	rm -rf $(OUTPUT) $(OUTPUT).iso $(FILE_SYSTEM)
